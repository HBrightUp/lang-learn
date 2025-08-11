#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<cmath>
#include<future>
#include"../include/thread.h"
#include"../include/thread_pool.hpp"
#include"../include/convert.h"



void func( CNode& node) {
    node.a = 10;
    node.b = 8;

    return ;
}

std::mutex mx;
void print_block(const int nums, const char c) {
    //mx.lock();
    std::lock_guard<std::mutex> lock(mx);

    for(int i = 0; i < nums; ++i) {
        std::cout << c;
    }

    std::cout << '\n';

    //mx.unlock();
}


 void test_thread_1() {
    CNode node;

    std::thread t(func, std::ref(node));
    t.join();

    std::cout << node.a << node.b << std::endl; //  108

 }

 void test_thread_2() {
    std::thread t1(print_block, 50, '#');
    std::thread t2(print_block, 50, '$');

    t1.join();
    t2.join();
 }


 void transfer(Box& from, Box& to, int num) {
    std::unique_lock<std::mutex> lock1(from.mx, std::defer_lock);
    std::unique_lock<std::mutex> lock2(to.mx, std::defer_lock);
    std::lock(lock1, lock2);

    from.num_things -= num;
    to.num_things += num;

    //num = 9;
 }
void test_thread_3() {
    Box b1(100);
    Box b2(50);

    const int coin = 10;
    std::thread t1(transfer, std::ref(b1), std::ref(b2), coin);
    std::thread t2(transfer, std::ref(b2), std::ref(b1), 4);
    t1.join(); t2.join();
    std::cout << "b1 num_things: " << b1.num_things << std::endl;   //  b1 num_things: 94
    std::cout << "b2 num_things: " << b2.num_things << std::endl;   //  b2 num_things: 56
}

std::condition_variable cv;
int cargo = 0;
bool shipment_avaiable() {
    return cargo != 0;
}
void consume(int n) {
    for(int i = 0; i < 10; ++i) {
        //std::cout << "111" << std::endl;
        std::unique_lock<std::mutex> lck(mx);
        //std::cout << "222" << std::endl;
        cv.wait(lck, shipment_avaiable);
        std::cout << "cargo: "<< cargo << ",i: " << i << std::endl;
        cargo = 0;
    }
}
void test_thread_4(){

    std::thread t(consume, 10);
    for(int i = 0; i < 10; ++i) {
        while(shipment_avaiable())
            std::this_thread::yield();
        
        std::unique_lock<std::mutex> lck(mx);
        cargo = i+1;
        cv.notify_one();
    }

     t.join();

    return ;
}

int value = 0;
void read_value() {
    std::cin >> value;
    cv.notify_one();
}
void test_thread_5() {
    std::cout << "Enter value: " << std::endl;

    std::thread t1(read_value);

    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);

    while(cv.wait_for(lck, std::chrono::seconds(1)) == std::cv_status::timeout) {
        std::cout << '.' << std::endl;
    }

    std::cout << "You entered: " << value << std::endl;
    t1.join();

    return ;
}


int g_deal_task = 0;

void CTask::process() {
    std::cout << "process+++" << std::endl;

    long i = 1000000000;
    while( i != 0) {
        int j = std::sqrt(i);
        --i;
    }
     std::cout << "process---" << std::endl;

     std::lock_guard lck(mx);
     std::cout << "deal task: " << ++g_deal_task << std::endl;
}

void test_thread_6() {

    CThreadPool<CTask> pool(6);
    const int max_task = 100;
    int total_task = 0;

    while(true) {
        CTask *t = new CTask();
        pool.append(t);
        std::cout << "task counts: " << pool.tasks_queue.size() << std::endl;
        delete t;

        std::cout << "total task: " << ++total_task << std::endl;
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
}

void test_thread_7() {
    std::promise<int> d;
    std::future<int> fut = d.get_future();
    
    std::thread t([&]() {
        d.set_value(10);
    });

    std::cout << "Future value: " << fut.get() << std::endl;    //  Future value: 10
    
    t.join();
}

void test_thread_8() {
    std::promise<CPrimiseTest> pm;
    std::future<CPrimiseTest> fut = pm.get_future();

    std::thread t1([&pm] {
        CPrimiseTest obj(10);
        pm.set_value(std::move(obj));
    });

    std::thread t2([&fut] {
        CPrimiseTest result = fut.get();
        std::cout << "Result: " << result.getValue() << std::endl;  //  Result: 10
    });

    t1.join(); t2.join();
}
void test_thread_9() {
    std::future<int> fut = std::async(std::launch::async, [](int x) {
        return x*x;
    }, 5);

    std::cout << "Result: " << fut.get() << std::endl;  //  Result: 25
 }

void call_thread() {
    std::cout << "call thread." << std::endl;


    //test_thread_9();

    call_convert();
}