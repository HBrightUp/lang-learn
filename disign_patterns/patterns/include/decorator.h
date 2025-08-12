#pragma once
#include<iostream>

class Component {
public:
    virtual void operation() = 0;
};

class ConcreteComponent : public Component {
public:
    void operation() override {
        std::cout << "ConcreteComponent::operation call." << std::endl;
    }
};

class Decorator : public Component {
protected:
    Component* component;
public:
    Decorator(Component* comp) : component(comp) {}
    void operation() override {
        component->operation();
        std::cout << "Decorator::operation call." << std::endl;
    }
};


class ConcreteDecoratorA : public Decorator {
public:
    ConcreteDecoratorA(Component* comp) : Decorator(comp) {}
    void operation() override {
        Decorator::operation();
        std::cout << "ConcreteDecoratorA::operation call." << std::endl;
    }
};

class ConcreteDecoratorB : public Decorator {
public:
    ConcreteDecoratorB(Component* comp) : Decorator(comp) {}
    void operation() override {
        Decorator::operation();
        std::cout << "ConcreteDecoratorB::operation call." << std::endl;
    }
};