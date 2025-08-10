#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
#include<set>
#include<map>
#include<deque>
#include<stack>
#include<list>
#include"../../common/include/template.hpp"
#include "../include/stl.h"

/******************************* vector *************************** */

void test_vecotr() {

    std::vector<int> v1;
    v1.assign(10, 8);
    print_vector(v1);

    int val = v1.at(2);
    std::cout << "val: " << val << std::endl;

    v1.clear();
    v1 = {1,2,3,4,5,6,7};
    print_vector(v1);
    std::cout << "capacity: " << v1.capacity() << std::endl;
    
    int *p = v1.data();
    print_array(p, v1.size());
    
    std::vector<int> v2 = {11, 12, 13, 14, 15,16};

    std::vector<int>::const_iterator it1 = v1.begin() + 3;
    v1.emplace(it1, 12);
    print_vector(v1);   //  1 2 3 12 4 5 6 7
    v1.emplace(it1, 13);
    print_vector(v1);   //  1 2 3 13 12 4 5 6 7
    v1.emplace_back(20);
    print_vector(v1); // 1 2 3 13 12 4 5 6 7 20

    v1.erase(it1);
    print_vector(v1);  // 1 2 3 12 4 5 6 7 20

    v1.erase(it1);
    print_vector(v1);  // 1 2 3 4 5 6 7 20
    v1.erase(v1.begin());
    print_vector(v1);   // 2 3 4 5 6 7 20
    v1.pop_back();
    print_vector(v1);   // 2 3 4 5 6 7

    std::cout << "max size: " << v1.max_size() << std::endl;

    v1.insert(it1, v2.begin(), v2.end());
    print_vector(v1);   //  2 3 4 11 12 13 14 15 16 5 6 7

    std::sort(v1.begin(), v1.end(), [](int a, int b) {
        return a > b;
    });
    print_vector(v1);

    //v1.reserve(3);
    std::cout << "capacity: " << v1.capacity() << std::endl;
    std::cout << "size: " << v1.size() << std::endl;
    print_vector(v1);   // 16 15 14 13 12 11 7 6 5 4 3 2

    v1.swap(v2);
    print_vector(v1);   //  11 12 13 14 15 16
    print_vector(v2);   //  16 15 14 13 12 11 7 6 5 4 3 2

    for( std::vector<int>::const_reverse_iterator it = v2.rbegin(); it != v2.rend(); ++it) {
        std::cout << *it << std::endl;  //  2 3 4 ...  14 15 16
    }

    std::cout << "test cap ++" << std::endl;
    std::vector<int> v3;
    for( int i = 0; i < 20; ++i ) {
        std::cout << "capacity: " << v3.capacity() << std::endl;
        std::cout << "size: " << v3.size() << std::endl;
        v3.emplace_back(i);
        print_vector(v3);
    }
    // capacity result: 0 -> 1 -> 2 -> 4 -> 8 -> 16 -> 32
}

void test_list() {


    std::list<int> l1;
    l1.assign(6, 8);    //  8 8 8 8 8 8 
    print_stl(l1);  

    l1.clear();

    l1 = {1,2,3,4,5,6};
    print_stl(l1);  //  1 2 3 4 5 6

    l1.emplace_front(20);
    print_stl(l1);  //  20 1 2 3 4 5 6 

    l1.erase(l1.begin());
    print_stl(l1);  //  1 2 3 4 5 6

    l1.insert(l1.begin(), 12);

    std::list<int> l2 = {25, 64};
    l1.merge(l2);
    print_stl(l1);  //  12 1 2 3 4 5 6 25 64

    l1.sort();
    print_stl(l1);

    l1.push_back(6);
    print_stl(l1);
    l1.sort();
    l1.unique();
    print_stl(l1);

    l1.

}


void test_vector() {

    //test_vecotr();
    test_list();
}