#pragma once

#include<iostream>

template<typename T> 
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template<typename T> 
void print_array(const T arr[], const unsigned int len) {

    for( int i = 0; i < len; ++i) {
        std::cout << arr[i] << " ";
    }

    std::cout << std::endl;
} 