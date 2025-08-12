#pragma once

#include <iostream>


class SubsystemA {
public:
    void OperationA() {
        std::cout << "SubsystemA: OperationA\n";
    }
};

class SubsystemB {
public:
    void OperationB() {
        std::cout << "SubsystemB: OperationB\n";
    }
};

// facade
class Facade {
private:
    SubsystemA* subsystemA;
    SubsystemB* subsystemB;

public:
    Facade() {
        subsystemA = new SubsystemA();
        subsystemB = new SubsystemB();
    }
    ~Facade() {
        delete subsystemA;
        delete subsystemB;
    }

    void Operation() {
        std::cout << "Facade: Operation start\n";
        subsystemA->OperationA();
        subsystemB->OperationB();
        std::cout << "Facade: Operation end\n";
    }
};

