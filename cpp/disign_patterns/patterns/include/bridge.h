#pragma once
#include <iostream>


class Implementor {
    public:
        virtual ~Implementor() = default;
        virtual void operationImpl() = 0;
};

class ConcreteImplementorA : public Implementor {
    public:
        void operationImpl() override {
            std::cout << "ConcreteImplementorA operation" << std::endl;
        }
};


class ConcreteImplementorB : public Implementor {
    public:
        void operationImpl() override {
            std::cout << "ConcreteImplementorB operation" << std::endl;
        }
};

class Abstraction {
protected:
    Implementor* impl;

public:
    Abstraction(Implementor* p) : impl(p) {}
    virtual ~Abstraction() = default;
    virtual void operation() = 0;
};

class RefinedAbstraction : public Abstraction {
public:
    RefinedAbstraction(Implementor* p) : Abstraction(p) {}
    void operation() override {
        impl->operationImpl();
    }
};

