#include "../include/animal.h"
#include<iostream>

void Animal::sound() const {
    std::cout << " animal sound." << std::endl;    
}

Animal::~Animal() {
    std::cout << " animal destoryed." << std::endl;    
}

