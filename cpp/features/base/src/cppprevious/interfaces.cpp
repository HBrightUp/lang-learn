#include<iostream>
#include"./include/interfaces.h"
#include"./include/object.h"

void cppprevious_call() {

    std::cout << "cppprevious::interfaces::call() recv." << std::endl;

    inherit();
}