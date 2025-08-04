#include<iostream>
#include"../include/concept.h"

template <typename T>
concept should_be_float = std::is_floating_point_v<T>;

template <typename T> requires should_be_float<T>
T add(T a, T b) { return a + b; }

void concept_example() {
    std::cout << "concept_example +++" << std::endl;

    auto r = add(1.1, 2.2);
    std::cout << r << std::endl;    // 3.3

    //auto r2 = add(1, 2);      // error, not add
}