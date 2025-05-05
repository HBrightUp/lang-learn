use flying::packages::{
    CrateName,
};
use flying::cli::{
    execute,
};

#[tokio::main]
async fn main() {
    execute(&CrateName::EnvLogger);
    println!("Hello, hml!");

    execute(&CrateName::Futures);

    std::thread::sleep(std::time::Duration::from_secs(10));
    println!("Hello, byebye! ");

    
}
