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
    
    const unsigned len = 19;
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
    int a7[len];
    memcpy(a7, a1, sizeof(a7));
    int a8[len];
    memcpy(a8, a1, sizeof(a8));
    int a9[len];
    memcpy(a9, a1, sizeof(a9));
    int a10[len];
    memcpy(a10, a1, sizeof(a10));

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

    std::cout << "quick sort" << std::endl;
    std::vector<int> v5(a5, a5 + len);
    quick_sort_non_recursive(v5);
    print_vector(v5);

    std::cout << "shell sort" << std::endl;
    shell_sort(a6, len);
    print_array(a6, len);

    std::cout << "heap sort" << std::endl;
    heap_sort(a7, len);
    print_array(a7, len);

    std::cout << "merge sort" << std::endl;
    //std::vector<int> v8(a8, a8 + len);
    merge_sort_non_recursive(a8, len);
    //print_vector(v8);
    print_array(a8, len);

    std::cout << "bucket sort" << std::endl;
    bucket_sort(a9, len);
    print_array(a9, len);

    std::cout << "radix sort" << std::endl;
    std::vector<int> v(a10, a10 + len);
    radix_sort(v);
    print_vector(v);
    
}

void call_algorithm() {
    //test_mylist();
    test_sort();
}