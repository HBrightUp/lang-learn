#include <iostream>
#pragma once

// 抽象基类
class AbstractClass {
public:
    // 模板方法，定义算法骨架
    void templateMethod() {
        primitiveOperation1();
        primitiveOperation2();
        concreteOperation();
        primitiveOperation3();
    }

    // 抽象方法，子类必须实现
    virtual void primitiveOperation1() = 0;
    virtual void primitiveOperation2() = 0;

    // 默认实现的方法，子类可以选择重写
    virtual void concreteOperation() {
        std::cout << "AbstractClass::concreteOperation()" << std::endl;
    }

    // 另一个抽象方法，子类必须实现
    virtual void primitiveOperation3() = 0;

    virtual ~AbstractClass() {}
};

// 具体子类1
class ConcreteClass1 : public AbstractClass {
public:
    void primitiveOperation1() override {
        std::cout << "ConcreteClass1::primitiveOperation1()" << std::endl;
    }

    void primitiveOperation2() override {
        std::cout << "ConcreteClass1::primitiveOperation2()" << std::endl;
    }

    void primitiveOperation3() override {
        std::cout << "ConcreteClass1::primitiveOperation3()" << std::endl;
    }
};

// 具体子类2
class ConcreteClass2 : public AbstractClass {
public:
    void primitiveOperation1() override {
        std::cout << "ConcreteClass2::primitiveOperation1()" << std::endl;
    }

    void primitiveOperation2() override {
        std::cout << "ConcreteClass2::primitiveOperation2()" << std::endl;
    }

    // 重写默认方法
    void concreteOperation() override {
        std::cout << "ConcreteClass2::concreteOperation()" << std::endl;
    }

     void primitiveOperation3() override {
        std::cout << "ConcreteClass2::primitiveOperation3()" << std::endl;
    }
};
