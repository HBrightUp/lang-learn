// use std::error::Error;
// use std::fs;
// use std::io::{BufReader, BufWriter};
// use std::path::Path;
 use futures::executor::block_on;
// use futures::future::{FutureExt, TryFutureExt};
// use reqwest::Client;

pub fn enter() {

    // method1: 
    // QUES: In order to prevent block_on from blocking the current thread, 
    //the future is deliberately put into the child thread to run. As a result, 
    //the reqwest::get() in the asynchronous task is always blocked.
    // tokio::spawn(async move  {
    //     let future_download = sample();
    //     block_on(future_download);
    // });

    //Method2: 
    let future_download = sample();
    block_on(future_download);
}

async fn sample() {

    println!("222");
    let url = "https://hyper.rs";
    let res = reqwest::get(url).await.unwrap();
    println!("222");
    println!("Response: {:?} {}", res.version(), res.status());
    println!("Headers: {:#?}\n", res.headers());

    let body = res.text().await.unwrap();

    println!("{body}");
}