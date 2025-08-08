#include<iostream>
#include<string.h>
#include"../include/algorithm.h"
#include"../../structures/include/list.hpp"
#include"../../common/include/generate.h"
#include"../include/sort.h"
#include"../../common/include/template.hpp"



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

void test_sort() {
    
    const unsigned len = 20;
    int a1[len];
    generate_rand_numbers(a1, len);
    int a2[len];
    memcpy(a2, a1, sizeof(a2));
    int a3[len];
    memcpy(a3, a1, sizeof(a3));
    int a4[len];
    memcpy(a4, a1, sizeof(a4));
    int a5[len];
    memcpy(a5, a1, sizeof(a5));
    int a6[len];
    memcpy(a6, a1, sizeof(a6));

    std::cout << "bubble sort" << std::endl;
    bubble_sort(a1, len);
    print_array(a1, len);
    
    std::cout << "cocktail bubble sort" << std::endl;
    cocktail_bubble_sort(a2, len);
    print_array(a2, len);

    std::cout << "selection sort" << std::endl;
    selection_sort(a3, len);
    print_array(a3, len);

    std::cout << "insert sort" << std::endl;
    selection_sort(a4, len);
    print_array(a4, len);

    // std::cout << "quick sort" << std::endl;
    // quick_sort(a5, 0, len-1);
    // print_array(a5, len);

    std::cout << "shell sort" << std::endl;
    shell_sort(a6, len);
    print_array(a6, len);
    

}

void call_algorithm() {
    //test_mylist();
    test_sort();
}