
use std::{

    thread::{
        self,LocalKey, sleep, JoinHandle,
    },
    sync::{
        Barrier, Arc, Mutex, Condvar, Once,
        mpsc:: {
            self, Sender, Receiver,
        },
        atomic::{
            AtomicU64, Ordering,AtomicBool, AtomicUsize
        },
    },
    cell::{
        RefCell,
    },
    time::{
        Duration,
    },
    time:: {
        Instant,
    },
    ops::{
        Sub,
    },
    hint::{
        self,
    },
};

static mut VAL: usize = 0;
static INIT: Once = Once::new();

const N_TIMES : u64 = 10000000;
const N_THREADS:usize = 10;
static R: AtomicU64 = AtomicU64::new(0);

static mut DATA: u64 = 0;
static READY: AtomicBool = AtomicBool::new(false);

struct Foo;

impl  Foo {
    thread_local!{
        static FOO: RefCell<usize> = RefCell::new(0);
    }
}

thread_local! {
    static FOO: RefCell<usize> = RefCell::new(100);
}
struct Bar {
    foo: &'static LocalKey<RefCell<usize>>,
}

impl Bar {
    fn constructor() -> Self {
        Self {
            foo: &FOO,
        }
    }
}

pub fn stdthread() {


    //Barrier
    {
        const  THREAD_NUM: usize  = 6;
        let mut handles = Vec::with_capacity(THREAD_NUM);
        let barrier = Arc::new(Barrier::new(THREAD_NUM));

        for _ in 0..THREAD_NUM {
            let b = barrier.clone();
            handles.push(thread::spawn( move || {
                println!("Before wait...");
                b.wait();
                println!("After wait...");
            }));
        }

        for handle in handles {
            handle.join().unwrap();
        }

        /*print:
            Before wait...
            Before wait...
            Before wait...
            Before wait...
            Before wait...
            Before wait...
            After wait...
            After wait...
            After wait...
            After wait...
            After wait...
            After wait...
        */
    }

    //thread_local!
    {
        {
            thread_local!(static FOO: RefCell<u32> = RefCell::new(1));

            FOO.with( |f| {
                assert_eq!(*f.borrow(), 1);
                *f.borrow_mut() = 2;
            });

            let handle = thread::spawn( move || {
                FOO.with( |f| {
                    assert_eq!(*f.borrow(), 1);
                    *f.borrow_mut() = 3;
                });
            });

            handle.join().unwrap();
            FOO.with( |f| {
                assert_eq!(*f.borrow(), 2);
            });
            
        }

        // use struct
        {
            Foo::FOO.with( |f| {
                //print: Foo: RefCell { value: 0 }
                println!("Foo: {:?}", f);
            });
        }
        
        // use struct with ref
        {
            let bar = Bar::constructor();
            bar.foo.with( |f| {
                assert_eq!(*f.borrow(), 100);
            });
        }
    }

    // condvar
    {
        let pair1 = Arc::new((Mutex::new(false), Condvar::new()));
        let pair2 = pair1.clone();

        thread::spawn( move || {
            let (lock, cvar) = &*pair2;
            let mut started = lock.lock().unwrap();
            println!("change started.");
            *started = true;
            cvar.notify_one();
        });

        let (lock, cvar) = &*pair1;
        let mut started = lock.lock().unwrap();
        while !*started {
            started = cvar.wait(started).unwrap();
        }

        println!("started changed.");

        /* print:
            change started.
            started changed.
         */
    }

    //Once 
    {
        let h1 = thread::spawn( || {
            INIT.call_once( || {
                unsafe {
                    VAL = 1;
                }
            });
        });

        let h2 = thread::spawn( || {
            INIT.call_once( || {
                unsafe {
                    VAL = 2;
                }
            });
        });

        h1.join().unwrap();
        h2.join().unwrap();

        // VAL is 1 or 2
        println!("VAL: {}", unsafe { VAL });
    }

    // mpsc 
    {
        // channel
        {
            let (tx, rx) = mpsc::channel();

            thread::spawn(move || {
                let vals = vec![
                    String::from("hi"),
                    String::from("from"),
                    String::from("the"),
                    String::from("thread"),
                ];
                
                for val in vals {
                    tx.send(val).unwrap();
                    thread::sleep(Duration::from_secs(1));
                }
            });
    
            for received in rx {
                println!("Got: {}", received);
            }
        }

        // channel with enum data
        {
            enum Fruit {
                Apple(u8),
                Orange(String),
            }

            let (tx, rx): (Sender<Fruit>, Receiver<Fruit>) = mpsc::channel();
            tx.send(Fruit::Orange("sweet".to_string())).unwrap();
            tx.send(Fruit::Apple(12)).unwrap();

            for _ in 0..2 {
                match rx.recv().unwrap() {
                    Fruit::Orange(flavor) => println!("Receive flavor orange: {}", flavor),
                    Fruit::Apple(count) => println!("Receive {} apple.", count),
                }
            }

        }

        //sync_channel
        {
            // with number of message cache,
            let (tx, rx)= mpsc::sync_channel(0);

            let handle = thread::spawn(move || {
                println!("before send");

                // Only after the message is received will the execution continue, otherwise it will be blocked forever.
                tx.send(1).unwrap();
                println!("afeter send");
            });

            println!("before sleep");
            thread::sleep(Duration::from_secs(3));
            println!("after sleep");

            println!("receive {}", rx.recv().unwrap());
            handle.join().unwrap();

            /* print:
                before sleep
                before send
                after sleep
                receive 1
                afeter send
             */
        }

       
    }

    //Mutex
    {
        {
            let counter = Arc::new(Mutex::new(0));
            let mut handles = vec![];

            for _ in 0..10 {
                let counter = counter.clone();
                let handle = thread::spawn( move || {
                    let mut num = counter.lock().unwrap();
                    *num += 1;
                });

                handles.push(handle);
            }

            for handle in handles {
                handle.join().unwrap();
            }

            // Result: 10
            println!("Result: {}", *counter.lock().unwrap());
        }
    }

    // Condvar
    {
        let flag = Arc::new(Mutex::new(false));
        let cond = Arc::new(Condvar::new());
        let cflag = flag.clone();
        let ccond = cond.clone();

        let h1 = thread::spawn( move || {
            let mut lock = cflag.lock().unwrap();
            let mut counter = 0;

            while counter < 3 {
                while !*lock {
                    lock = ccond.wait(lock).unwrap();
                }

                *lock = false;
                counter += 1;
                println!("Inner counter: {}", counter);
            }
        });

        let mut counter = 0;
        loop {
            sleep(Duration::from_millis(1000));
            *flag.lock().unwrap() = true;
            counter += 1;
            if counter > 3 {
                break;
            }

            println!("outside counter: {}", counter);
            cond.notify_one();
        }

        h1.join().unwrap();

        println!("flag: {:?}", flag);

        /*print:
        outside counter: 1
            Inner counter: 1
            outside counter: 2
            Inner counter: 2
            outside counter: 3
            Inner counter: 3
            flag: Mutex { data: true, poisoned: false, .. }
        
         */
    }

    // Atomic
    {
        {
            let s = Instant::now();
            let mut threads = Vec::with_capacity(N_THREADS);
            
            for _ in 0..N_THREADS {
                threads.push(add_n_times(N_TIMES));
            }

            for thread in threads {
                thread.join().unwrap();
            }

            assert_eq!(N_THREADS * N_TIMES as usize, R.load(Ordering::Relaxed) as usize);
            println!("speed time: {:?}", Instant::now().sub(s));

        }

        // memory ordering:  Release/Acquire
        {
            reset();
    
            let t_producer = producer();
            let t_consumer = consumer();
    
            t_producer.join().unwrap();
            t_consumer.join().unwrap();
            
        }

        //SeqCst: 
        {
            let spinlock = Arc::new(AtomicUsize::new(1));
    
            let spinlock_clone = Arc::clone(&spinlock);
            let thread = thread::spawn(move|| {
                println!("111");
                spinlock_clone.store(0, Ordering::SeqCst  );
                println!("222");
            });
    
            println!("333");
            // 等待其它线程释放锁
            while spinlock.load(Ordering::SeqCst  ) != 0 {
                println!("444");
                hint::spin_loop();
                println!("555");
            }
            println!("666");
            if let Err(panic) = thread.join() {
                println!("Thread had an error: {:?}", panic);
            }
        }
    }

}

fn add_n_times(n: u64) -> JoinHandle<()> {
    thread::spawn( move || {
        for _ in 0..n {
            R.fetch_add(1, Ordering::Relaxed);
        }
    })
}

fn reset() {
    unsafe {
        DATA = 0;
    }
    READY.store(false, Ordering::Relaxed);
}

fn producer() -> JoinHandle<()> {
    thread::spawn(move || {
        unsafe {
            DATA = 100;                                 // A
        }
        READY.store(true, Ordering::Release);           // B: 内存屏障 ↑
    })
}

fn consumer() -> JoinHandle<()> {
    thread::spawn(move || {
        while !READY.load(Ordering::Acquire) {}         // C: 内存屏障 ↓

        assert_eq!(100, unsafe { DATA });               // D
    })
}