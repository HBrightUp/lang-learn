
use log::{debug, error, log_enabled, info, Level};
use std::env;
use std::thread;



pub fn enter() {
    
    init();
    //test();

}

pub fn init() {
    unsafe{
        env::set_var("RUST_LOG", "debug");
    };
   
    env_logger::init();
}

fn test() {
    debug!("this is a debug {}", "message");
    error!("this is printed by default");
    
    if log_enabled!(Level::Info) {
        let x = 3 * 4; // expensive computation
        info!("the answer was: {}", x);
    }

    let mut handles = vec![];
    for i in 0..10 {
        let handle = thread::spawn( move || {

            info!("index: {}", i);
            debug!("this is a debug {}", i);
            error!("this is printed by default:{}", i);
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }
}