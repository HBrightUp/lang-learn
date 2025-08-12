#pragma once

#include <iostream>
#include <vector>
#include <string>

// 抽象组件
class ComponentA {
public:
    virtual void operation() = 0;
    virtual void add(ComponentA* ComponentA) {}
    virtual void remove(ComponentA* ComponentA) {}
    virtual ComponentA* getChild(int index) { return nullptr; }
    virtual ~ComponentA() {}
};

// 叶子节点
class Leaf : public ComponentA {
public:
    Leaf(const std::string& name) : name_(name) {}
    void operation() override {
        std::cout << "Leaf " << name_ << " operation" << std::endl;
    }
private:
    std::string name_;
};

// 容器节点
class Composite : public ComponentA {
public:
    void operation() override {
        std::cout << "Composite operation" << std::endl;
        for (ComponentA* child : children_) {
            child->operation();
        }
    }
    void add(ComponentA* ComponentA) override {
        children_.push_back(ComponentA);
    }
    void remove(ComponentA* ComponentA) override {
        // ... (移除component的代码)
    }
    ComponentA* getChild(int index) override {
        if (index >= 0 && index < children_.size()) {
            return children_[index];
        }
        return nullptr;
    }
private:
    std::vector<ComponentA*> children_;
};
