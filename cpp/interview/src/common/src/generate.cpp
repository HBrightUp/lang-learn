#include<iostream>
#include<random>
#include"../include/generate.h"
#include"../include/template.hpp"


void generate_rand_numbers(int arr[], const unsigned len) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 100);
    unsigned int pos = len;

    
    while(pos--) {
        arr[pos] = distrib(gen);
    }

    print_array(arr, len);
  
}
