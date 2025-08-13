#pragma once

#include <iostream>

// CTarget
class CTarget {
    public:
        virtual void request() {
            std::cout << "CTarget::request()" << std::endl;
        }
        virtual ~CTarget() { std::cout << "CTarget destructor call." << std::endl; }
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
class CAdapter : public CTarget, public Adaptee {
    public:
        void request() override {
            specificRequest(); // call  Adaptee::specificRequest();
        }
        virtual ~CAdapter() { std::cout << "CAdapter destructor call." << std::endl; }
};

// Method-2: create a object of class Adaptee.
class ObjectAdapter : public CTarget {
    private:
        Adaptee* adaptee;
    public:
        ObjectAdapter(Adaptee* ada) : adaptee(ada) {}
        void request() override {
            adaptee->specificRequest(); // call  Adaptee::specificRequest();
        }
        virtual ~ObjectAdapter() { std::cout << "ObjectAdapter destructor call." << std::endl; }
};

