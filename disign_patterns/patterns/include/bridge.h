#include <iostream>

// 实现化接口 (Abstraction Interface)
class Implementor {
public:
    virtual ~Implementor() = default;
    virtual void operationImpl() = 0;
};

// 具体实现化类 A
class ConcreteImplementorA : public Implementor {
public:
    void operationImpl() override {
        std::cout << "ConcreteImplementorA operation" << std::endl;
    }
};

// 具体实现化类 B
class ConcreteImplementorB : public Implementor {
public:
    void operationImpl() override {
        std::cout << "ConcreteImplementorB operation" << std::endl;
    }
};

// 抽象化类 (Abstraction)
class Abstraction {
protected:
    Implementor* impl;

public:
    Abstraction(Implementor* p) : impl(p) {}
    virtual ~Abstraction() = default;
    virtual void operation() = 0;
};

// 扩展抽象化类
class RefinedAbstraction : public Abstraction {
public:
    RefinedAbstraction(Implementor* p) : Abstraction(p) {}
    void operation() override {
        impl->operationImpl();
    }
};

