#pragma once

#include <iostream>

// target
class Target {
public:
    virtual void request() {
        std::cout << "Target::request()" << std::endl;
    }
    virtual ~Target() { std::cout << "Target destructor call." << std::endl; }
};

// source
class Adaptee {
public:
    void specificRequest() {
        std::cout << "Adaptee::specificRequest()" << std::endl;
    }
    virtual ~Adaptee() { std::cout << "Adaptee destructor call." << std::endl; }
};

// Methon-1: create new class 
class ClassAdapter : public Target, public Adaptee {
public:
    void request() override {
        specificRequest(); // call  Adaptee::specificRequest();
    }
    virtual ~ClassAdapter() { std::cout << "ClassAdapter destructor call." << std::endl; }
};

// Method-2: create a object of class Adaptee.
class ObjectAdapter : public Target {
private:
    Adaptee* adaptee;
public:
    ObjectAdapter(Adaptee* ada) : adaptee(ada) {}
    void request() override {
        adaptee->specificRequest(); // call  Adaptee::specificRequest();
    }
    virtual ~ObjectAdapter() { std::cout << "ObjectAdapter destructor call." << std::endl; }
};

