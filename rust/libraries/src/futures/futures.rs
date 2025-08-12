use log::{debug, info};
use futures::{
    future::{ self, join, MaybeDone},
    executor,
};
use std::time::Instant;


pub fn enter() {
    debug!("Module futures enter ...");

    let fur_join = futures_join();
    executor::block_on(fur_join);

    let fur_lazy = futures_lazy();
    executor::block_on(fur_lazy);

    let fut_maybe = futures_maybe_done();
    executor::block_on(fut_maybe);
}

async fn futures_maybe_done() {
    let start_time = Instant::now();

    let fur_task = excute_task();
    let maybe_done: MaybeDone<_> = future::maybe_done(fur_task);

    let result = maybe_done.await;
    
    info!(" Task finished, the result: {:?}, speed time: {:?}",result, start_time.elapsed());
}


//join/join3/ ... /join_all
async fn futures_join() {
    let a = async { 1 };
    let b = async { 2 };

    let pair = future::join(a, b );

    assert_eq!(pair.await, ( 1, 2));
}

async fn futures_lazy() {
   
    let mut c = future::lazy(  |_| -> usize {
           888
    });

    let sum = c.await;

    info!("sum: {sum}");

}

async fn excute_task() -> usize {
    tokio::time::sleep(tokio::time::Duration::from_secs(6)).await;
    99 as usize
}


