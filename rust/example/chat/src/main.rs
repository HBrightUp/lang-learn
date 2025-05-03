use tokio::net::{TcpListener, TcpStream};
use tokio::sync::{mpsc, Mutex};
use tokio_stream::StreamExt;
use tokio_util::codec::{Framed, LinesCodec};

use futures::SinkExt;
use std::collections::HashMap;
use std::env;
use std::error::Error;
use std::io;
use std::net::SocketAddr;
use std::sync::Arc;
use std::thread;
use std::time::Duration;


#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    use tracing_subscriber::{fmt::format::FmtSpan, EnvFilter};

    tracing_subscriber::fmt()
        .with_env_filter(EnvFilter::from_default_env().add_directive("chat=debug".parse()?))
        .with_span_events(FmtSpan::FULL)
        .init();

    let state = Arc::new(Mutex::new(Shared::new()));

    let addr = env::args()
        .nth(1)
        .unwrap_or_else(|| "127.0.0.1:6142".to_string());

    let listener = TcpListener::bind(&addr).await?;
    tracing::info!("server running on {}", addr);

    loop {
        let (stream, addr) = listener.accept().await?;
        let state = Arc::clone(&state);
        tokio::spawn(async move {
            tracing::debug!("accepted connection: {}",addr);
            if let Err(e) = process(state, stream, addr).await {
                tracing::info!("an error occurred; error = {:?}", e);
            }
        });
    }
}


type Tx = mpsc::UnboundedSender<String>;
type Rx = mpsc::UnboundedReceiver<String>;

//SocketAddr: clinet ip and port
// Tx:  each client have a mpsc and save tx by self.
struct Shared {
    peers: HashMap<SocketAddr, Tx>,
}

struct Peer {
    //TcpStream: client ip and port.
    lines: Framed<TcpStream, LinesCodec>,

    // rx of client
    rx: Rx,
}

impl Shared {
    fn new() -> Self {
        Shared {
            peers: HashMap::new(),
        }
    }

    async fn broadcast(&mut self, sender: SocketAddr, message: &str) {
        for peer in self.peers.iter_mut() {
            if *peer.0 != sender {
                let _ = peer.1.send(message.into());
            }
        }
    }
}

impl Peer {
    async fn new(
        state: Arc<Mutex<Shared>>,
        lines: Framed<TcpStream, LinesCodec>,
    ) -> io::Result<Peer> {

        // This addr is the same as the parameter addr in process, both are client ip:port addresses.
        let addr = lines.get_ref().peer_addr()?;
        tracing::debug!("Peer new() addr: {}",addr);

        let (tx, rx) = mpsc::unbounded_channel();
        state.lock().await.peers.insert(addr, tx);
        Ok(Peer { lines, rx })
    }
}

async fn process(
    state: Arc<Mutex<Shared>>,
    stream: TcpStream,
    addr: SocketAddr,
) -> Result<(), Box<dyn Error>> {
    let mut lines = Framed::new(stream, LinesCodec::new());
    lines.send("Please enter your username:").await?;
    let username = match lines.next().await {
        Some(Ok(line)) => line,
        _ => {
            tracing::error!("Failed to get username from {}. Client disconnected.", addr);
            return Ok(());
        }
    };

    let mut peer = Peer::new(state.clone(), lines).await?;

    {
        let mut state = state.lock().await;
        let msg = format!("{username} has joined the chat");
        tracing::info!("{}", msg);
        state.broadcast(addr, &msg).await;
    }

    loop {
        //Select  router to receive the message sent by mpsc tx in round-robin
        tokio::select! {
           
            //recived the broadcast from tx 
            Some(msg) = peer.rx.recv() => {

                //Warinning:  Here just for debug.Although the delay is done here, 
                //the client still receives the data at the same time, indicating that tokio::select is concurrent here.
                // let addr = peer.lines.get_ref().peer_addr()?;
                // tracing::Debug!("peer tx: {addr} send, select rx receive: {}", msg);
                // thread::sleep(Duration::from_secs(10));

                // send msg to other client.
                peer.lines.send(&msg).await?;
            }

            // waitting fro message for single client.
            result = peer.lines.next() => match result {
                Some(Ok(msg)) => {
                    let mut state = state.lock().await;
                    let msg = format!("{username}: {msg}");

                    state.broadcast(addr, &msg).await;
                }
                Some(Err(e)) => {
                    tracing::error!(
                        "an error occurred while processing messages for {}; error = {:?}",
                        username,
                        e
                    );
                }
                None => break,
            },
        }
    }

    {
        let mut state = state.lock().await;
        state.peers.remove(&addr);

        let msg = format!("{username} has left the chat");
        tracing::info!("{}", msg);
        state.broadcast(addr, &msg).await;
    }

    Ok(())
}
