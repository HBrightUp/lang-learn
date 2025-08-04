#include "../include/dog.h"
#include<iostream>

void Dog::sound() const {
    std::cout << " Dog sound." << std::endl;    
}

Dog::~Dog() {
    std::cout << " Dog destoryed." << std::endl;    
}

