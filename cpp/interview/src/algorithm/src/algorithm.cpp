#include<iostream>
#include"../include/algorithm.h"
#include"../../structures/include/list.hpp"



void test_mylist() {

    MyList<int> list;


    list.create_list(1);
    list.print();

    list.insert(0, 2);
    list.print();

    list.insert(1, 3);
    list.print();

    list.insert(10, 4);
    list.print();
    
    list.update(1, 10);
    list.print();

    list.reverse();
    list.print();
}

void call_algorithm() {
    test_mylist();
    
}