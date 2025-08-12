use tokio::net::TcpStream;
use tokio::io::AsyncWriteExt;
use std::error::Error;
use futures::executor::block_on;
use crate::tokio::echo_tcp;
use crate::tokio::connect_tcp;

 pub  fn enter() {

   
    
   // call_hello_world();
        call_echo_tcp();

}

// start Tcp server and client.
pub fn call_echo_tcp() {
    
    tokio::spawn(async move  {
        let future_echo_tcp = echo_tcp::echo_tcp();
        block_on(future_echo_tcp);
    });

    let future_connect_tcp = connect_tcp::connect_tcp();
    block_on(future_connect_tcp);
}

// hello test
pub fn call_hello_world() {
    let future_hello = hello_world();
    let _ = block_on(future_hello);
}

pub async fn hello_world() -> Result<(), Box<dyn Error>> {

    let mut stream = TcpStream::connect("127.0.0.1:22").await?;
    println!("Create stream.");

    let result = stream.write_all(b"hello, world\n").await;
    println!("Wrote to stream; successful={:?}", result.is_ok());

    Ok(())
}

// other method
//tokio::try_join!(Fn1, Fn2, ..);  : Waiting for the execution results of multiple asynchronous tasks.