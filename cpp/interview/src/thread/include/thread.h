#pragma once
#include<mutex>

class CNode {
    public:
        int a;
        int b;
};


struct Box{
    int num_things;
    std::mutex mx;

    explicit Box(int num): num_things(num) {}
    
};

class CTask{
    public:
        void process();
};

class CPrimiseTest {
    public:
        CPrimiseTest(int val):value(val) {}
        CPrimiseTest(const CPrimiseTest& other): value(other.value) {
            std::cout << "Copy constructor called." << std::endl;
        }

        CPrimiseTest(CPrimiseTest&& other): value(other.value) {
            std::cout << "Move constructor called." << std::endl;
            other.value = 0;
        }

        CPrimiseTest& operator=(  CPrimiseTest& other) {
            if (this != &other) {
                value = other.value;
                std::cout << "Copy assign constructor called." << std::endl;
            }
            return *this;
        }

        CPrimiseTest& operator=(CPrimiseTest&& other) {
            if (this != &other) {
                value = other.value;
                other.value = 0;
                std::cout << "Move assign constructor called." << std::endl;
            }
            return *this;
        }
        
        int getValue() const {return value;}

    private:
        int value;
};











void call_thread();