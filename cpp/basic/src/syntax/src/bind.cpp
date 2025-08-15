#include<iostream>
#include<functional>
#include<thread>
#include"../include/bind.h"


void print_name(const std::string& name) {
    std::cout << "Name: " << name  << std::endl;
}

//1. Delayed execution
void delay_execute() {
    auto print = std::bind(print_name, "Bright");
    print();
}

//Simplify callback or thread functions
void simplify_function() {

    std::thread t(std::bind(print_name, "hml"));

    std::this_thread::sleep_for(std::chrono::seconds(1));

    t.join();
}

// using placeholders
void multiply(int a, int b, int c) {
    std::cout << "a: " << a << " c: " << c << std::endl;    //  a: 5 c: 3
    std::cout << "Result: " << a * b * c << std::endl;  //  60
}   

void with_placeholders() {
    std::function<void(int, int)> mult = std::bind(multiply, std::placeholders::_2, 4, std::placeholders::_1);
    mult(3,5);
}


class MyClass{
    public:
        void print_sum(int a, int b) {
            std::cout << "sum: " << a + b << std::endl; 
        }
};

void with_class_member() {
    MyClass obj;
    auto print = std::bind(&MyClass::print_sum, &obj, 10, std::placeholders::_1);
    print(65);    //  75
}

void call_bind() {

    delay_execute();
    simplify_function();
    with_placeholders();
    with_class_member();
}