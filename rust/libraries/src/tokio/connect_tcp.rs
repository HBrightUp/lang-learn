
use tokio::net::TcpStream;
use std::net::SocketAddr;
use tokio::io::AsyncWriteExt;
use tokio::io::AsyncReadExt;


pub async fn connect_tcp() {
    
    let addr: String = "127.0.0.1:8080".to_string();
    let addr = addr.parse::<SocketAddr>().unwrap();


    for i in 0..5 {
        
        tokio::spawn( async move {
            let mut stream = TcpStream::connect(addr).await.unwrap();

            //Warning: The function 'write_all' must be added with 'await', otherwise the data received by the TCP server  will be empty.
            
            let content = format!("cliend message: {}", i);
           let _ = stream.write_all(content.as_bytes()).await;

            let mut buf = vec![0; 32];
            let len = stream.read(&mut buf).await.expect("client failed to read.");
            println!("client receive data: {:?}", &buf[0..len]);
            
        });
    }


}
    
