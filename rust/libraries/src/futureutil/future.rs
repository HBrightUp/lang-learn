use futures::channel::mpsc;
use futures::executor; 
use futures::executor::ThreadPool;
use futures::StreamExt;

pub fn enter() {
    fuctional();
    imperative();
}


fn fuctional() {
    
    let pool =  ThreadPool::new().expect("Failed to build pool");
    let (tx, rx) = mpsc::unbounded::<i32>();

    let fut_value = async {
  
        let fut_tx_result = async move {
            (0..100).for_each( | v | {
                tx.unbounded_send(v).expect("Failed to send.");
            })
        };

        // execute this future "fut_tx_result".
        pool.spawn_ok(fut_tx_result);

        let fut_value = rx.map( |v| v * 2).collect();
       
        fut_value.await
    };

    let values: Vec<i32> = executor::block_on(fut_value);

    println!("values: {:?}", values);
}

fn imperative() {

    let pool = ThreadPool::new().expect("Failed to build pool");
    let (tx, mut rx) = mpsc::unbounded::<i32>();

    let fut_values = async {
        let fut_tx_result = async move {
            (0..100).for_each(|v| {
                tx.unbounded_send(v).expect("Failed to send");
            })
        };

        pool.spawn_ok(fut_tx_result);

        let mut pending = vec![];
   
        while let Some(v) = rx.next().await {
            pending.push(v * 2);
        }

        pending
    };

    let values: Vec<i32> = executor::block_on(fut_values);
    println!("Values={values:?}");
}