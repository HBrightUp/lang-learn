#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
#include<set>
#include<map>
#include<unordered_map>
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

    for(std::list<int>::const_reverse_iterator it = l1.rbegin(); it != l1.rend(); ++it)
     {
        std::cout << *it << " ";
     }

}

template<typename T>
void print_person_map(const T& m) {
   for(const auto& v : m) {
        std::cout << v.first << " " << v.second.age << " " << v.second.name << std::endl;
   }
}
void test_map() {
    std::map<int, Person>  p1;
    int id = 0;

    p1.emplace(id++, Person{
        10,
        "name1"
    });

    p1.emplace(id++, Person{
        8,
        "name2"
    });

    p1.emplace(id++, Person{
        13,
        "name3"
    });

    p1.emplace(id++, Person{
        11,
        "name4"
    });

    print_person_map(p1);

    const auto& e1 = p1.count(1);
    std::cout << e1 << std::endl;

    const auto e2 = p1.find(6);
    bool is_exist = e2 != p1.end();
    std::cout << is_exist << std::endl;

    auto result = p1.emplace(id, Person{
        21,
        "name43"
    });
    if (result.second) {
         std::cout << "emplace success!" << std::endl;
    } else {
        std::cout << "emplace failed!" << std::endl;
    }
    
    print_person_map(p1);

    result = p1.emplace(id++, Person{
        22,
        "name23"
    });
     
    if (result.second) {
         std::cout << "emplace success!" << std::endl;
    } else {
        std::cout << "emplace failed!" << std::endl;    // enter
        std::cout << result.first->first << " " << result.first->second.age << " "<< result.first->second.name << std::endl;
    }
    print_person_map(p1);

    p1.try_emplace(id, Person {
        65,
        "hhh"
    });
    print_person_map(p1);

    result = p1.try_emplace(id, Person {
        63,
        "BBB"
    });

    if (result.second) {
         std::cout << "emplace success!" << std::endl;  // enter
    } else {
        std::cout << "emplace failed!" << std::endl;
    }

     print_person_map(p1);
}

void test_multimap() {
    std::multimap<int, Person> p1;

    int id = 0;

    p1.emplace(id++, Person{
        10,
        "name1"
    });

    p1.emplace(id++, Person{
        8,
        "name2"
    });

    p1.emplace(id, Person{
        13,
        "name3"
    });

    p1.emplace(id++, Person{
        11,
        "name4"
    });
      p1.emplace(id++, Person{
        56,
        "name5"
    });

    print_person_map(p1);

    std::cout << p1.count(2) << std::endl;      //  2

    auto  result = p1.equal_range(2);
    std::cout << result.first->first << " "<< result.second->first << std::endl;    //  2 3
    std::cout << result.first->second.age << " " << result.first->second.name << std::endl; //  13 name3

    auto r2 = p1.lower_bound(2);
    std::cout << r2->first << std::endl;    //  2

    auto r3 = p1.upper_bound(2);
    std::cout << r3->first << std::endl;    //  3
    print_person_map(p1);

    p1.clear();

}

void test_unordered_map() {
    std::unordered_map<int, int> m;

    const int len = 10000;
    for(int i = 0; i < len; ++i) {
         m.emplace(i,i);
         
    }

    m.rehash(10);
    std::cout << "bucket count: " << m.bucket_count() << std::endl;
    std::cout << "bucket size: " << m.bucket_size(1) << std::endl;
    std::cout << "bucket number: " << m.bucket(10) << std::endl;
    std::cout << "load factor: " << m.load_factor() << std::endl;


    std::unordered_map<Person, int> m2;


    Person p1 = {1, "hml"};
    Person p2 = {2, "hk"};
    Person p3 = {3, "bb"};
    //m2[p] = 32;

    m2[p1] = 100;
    m2[p2] = 158;
    m2[p3] = 3655;

    std::cout << m2[p2] << std::endl;   //  158

    
}

void test_set() {

    std::set<int> s1;

    s1.emplace(21);
    s1.emplace(3);
    s1.emplace(5);
    s1.emplace(13);
    print_stl(s1);  //  3 5 13 21 

    const auto& it = s1.find(5);
    s1.emplace_hint(it, 63);
    print_stl(s1);  //  3 5 13 21 63

    const auto& it2 = s1.equal_range(13);
    std::cout << *it2.first << "  " << *it2.second << std::endl;    //  13 21

    std::cout << s1.key_comp()(120, 58) << std::endl;       //  0
    std::set<int> s2 = {32,43,453};
    s1.merge(s2);

    print_stl(s1);  //  3 5 13 21 32 43 63 453 
    print_stl(s2);  //  empty element

    s1.clear();
}

void test_multiset() {

    std::multiset<int> s;

    s.emplace(55);
    s.emplace(12);
    s.emplace(89);
    s.emplace(35);
    s.emplace(12);
    s.insert(55);  

    print_stl(s);   //  12 12 35 55 55 89 

    auto node = s.extract(55);
    node.value() = 20;
    print_stl(s);   //  12 12 35 55 89
    s.extract(55);
    print_stl(s);   //  12 12 35 89
    s.extract(12);
    print_stl(s);   //  12 35 89
}

void test_stl() {

    //test_vecotr();
    //test_list();
    //test_map();
    //test_multimap();
    //test_unordered_map();
    //test_set();
    test_multiset();
}