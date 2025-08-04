#include "../include/cat.h"
#include<iostream>

void Cat::sound() const {
    std::cout << " Cat sound." << std::endl;    
}

Cat::~Cat() {
    std::cout << " Cat destoryed." << std::endl;    
}

