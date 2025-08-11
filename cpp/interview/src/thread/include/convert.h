
#pragma once
#include<iostream>
#include <typeinfo>

class Base {
public:
    virtual void print() { std::cout << "Base class" << std::endl; }
    virtual ~Base() { std::cout << "Base destruct " << std::endl;} // 
};

class Derived : public Base {
public:
    void print() override { std::cout << "Derived class" << std::endl; }
    void derivedMethod() { std::cout << "Derived method" << std::endl; }
    virtual ~Derived() { std::cout << "Derive destructor" << std::endl;}
};






void call_convert();