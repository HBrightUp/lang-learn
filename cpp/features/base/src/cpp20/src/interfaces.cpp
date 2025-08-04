#include<iostream>
#include"../include/interfaces.h"
#include"../../common/include/source_location.h"
#include"../include/concept.h"



void cpp20_call() {
    std::cout << "cpp20_call +++" << std::endl;
    
    concept_example();
 }