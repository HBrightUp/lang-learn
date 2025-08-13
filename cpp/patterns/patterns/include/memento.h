#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// 备忘录 (Memento)
class Memento {
private:
    std::string state;
public:
    Memento(const std::string& s) : state(s) {}
    std::string getState() const { return state; }
};

// 发起人 (Originator)
class Originator {
private:
    std::string state;
public:
    void setState(const std::string& s) { state = s; }
    std::string getState() const { return state; }
    Memento* createMemento() { return new Memento(state); }
    void restoreMemento(const Memento* m) { state = m->getState(); }
};

// 管理者 (Caretaker)
class Caretaker {
private:
    std::vector<Memento*> mementos;
public:
    void addMemento(Memento* m) { mementos.push_back(m); }
    Memento* getMemento(int index) { return mementos[index]; }
    ~Caretaker() {
        for (auto m : mementos) {
            delete m;
        }
    }
};

