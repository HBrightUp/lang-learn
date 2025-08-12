
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::net::TcpListener;

pub async fn echo_tcp() {
    
    let addr = "127.0.0.1:8080".to_string();

    println!("Listen on: {addr} ");

    let listener = TcpListener::bind(&addr).await.unwrap();

    loop {
        let (mut socket, _) = listener.accept().await.unwrap();
        tokio::spawn( async move {
            let mut buf = vec![0; 32];

            loop {
                let n = socket.read(&mut buf).await.expect("faild read data from socket.");
                println!("Receive data from client, data: {:?}", &buf[0..n]);
                println!("n: {}", n);
                if n == 0 {
                    return ;
                }

                socket.write_all(&buf[0..n]).await.expect("failed write data to socket.");
            }
        } );
    }
}