#pragma once 

#include <iostream>
#include <string>
#include <memory>


class Prototype {
public:
    virtual std::unique_ptr<Prototype> clone() const = 0;
    virtual void show() const = 0;
    virtual ~Prototype() = default;
};


class ConcretePrototypeA : public Prototype {
private:
    std::string data;
public:
    ConcretePrototypeA(const std::string& data) : data(data) {}

    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<ConcretePrototypeA>(*this); // Deep Copy
    }

    void show() const override {
        std::cout << "ConcretePrototypeA: " << data << std::endl;
    }
};