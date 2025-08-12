#[tokio::main]
async fn main() -> Result<(), reqwest::Error> {

    get_url_data().await;

    Ok(())
}

async fn get_url_data() {

    let url = "https://hyper.rs";
    let res = reqwest::get(url).await.unwrap();
    println!("222");
    println!("Response: {:?} {}", res.version(), res.status());
    println!("Headers: {:#?}\n", res.headers());

    let body = res.text().await.unwrap();

    println!("{body}");
}