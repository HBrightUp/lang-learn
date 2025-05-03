use flying::packages::{
    CrateName,
};
use flying::cli::{
    execute,
};

#[tokio::main]
async fn main() {
    //execute(&CrateName::EnvLogger);
    println!("Hello, flying!");

    execute(&CrateName::Secp256k1);

    std::thread::sleep(std::time::Duration::from_secs(2));
    println!("Hello, byebye! ");

    
}
