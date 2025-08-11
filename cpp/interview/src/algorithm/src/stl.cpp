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
#include<forward_list>
#include<array>
#include<deque>
#include<queue>
#include<random>
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


void test_forward_list() {

    std::forward_list<int> l1 = {2,3,7,3,62,3,6,1};

    print_stl(l1);

    //l1.assign(12,12);
    l1.emplace_front(58);
    print_stl(l1);  //  58 2 3 7 3 62 3 6 1 

    l1.remove_if([](int a) {
        return a % 2 == 0;
    });
    print_stl(l1);  //3 7 3 3 1 

    l1.unique();
    print_stl(l1);  //  3 7 3 1
    
}

void test_array() {
    std::array<int, 5> a1 = {2,6,3,9,7};
    print_stl(a1);

    a1.fill(3);
    print_stl(a1);//    3 3 3 3 3

    std::cout << a1.empty() << a1.size() << std::endl;  //  05

}

void test_stack() {

    std::stack<int> s;

    s.emplace(12);
    s.emplace(32);
    s.push(23);


    std::cout << s.empty() << s.size() << s.top() << std::endl; //  0323
    s.pop();
    std::cout << s.top() << std::endl;  //  32
}

void test_deque() {
    std::deque<int> d;

    d.assign(5,6);
    print_stl(d);   //  6 6 6 6 6

    d = {2,3,4,5,6};
    print_stl(d);   //  2 3 4 5 6 
    
    d.emplace(d.begin() + 3, 23);
    print_stl(d);   //  2 3 4 23 5 6

    std::cout << d.front() << std::endl;    //  2

    d.pop_back();
    print_stl(d);   //  2 3 4 23 5 

    std::cout << d.size() << std::endl; /// 5

    d.shrink_to_fit();
}

void test_queue() {
    std::queue<int> q;

    q.emplace(12);
    q.push(32);
    q.push(56);

    std::cout << q.size() << q.front() << std::endl;    //  312

    q.pop();
    std::cout << q.front() << std::endl;    //32
}

void test_priority_queue() {

    struct Friut {
        std::string name;
        int price;

        Friut(std::string n, int p):name(n), price(p) {}
        
        bool operator<(const Friut& other) const {
            return price < other.price;
        }

        

    };

    std::priority_queue< Friut> q;

    Friut f1("apple", 8);
    Friut f2("peach", 5);
    Friut f3("watermelon", 4);

    q.emplace(f1);
    q.push(f2);
    q.push(f3);


    while(!q.empty()) {
        std::cout << "name: " << q.top().name << ", price: " << q.top().price << std::endl;  
        q.pop();
    }   

}

void test_stl_algorithm() {

    std::multiset<int> s1 = {55, 46, 27, 46,5, 62, 74, 34, 753, 34, 27, 55, 42,42,42,46}; 
    print_stl(s1);  //  5 27 27 34 34 42 42 42 46 46 46 55 55 62 74 753

    std::vector<int> v1 = {32, 65, 67, 42, 27, 34, 46, 87};
    std::vector<int> v2 = {22, 65, 67, 42, 27, 34, 47, 88, 88, 32};

    //  1. find(begin, end, value)
    auto it1 = std::find(s1.begin(), s1.end (), 27);
    if (it1 != s1.end()) {
        std::cout << "find: " << *it1 << std::endl;  //  find: 27
    }

    // 2. find_first_of(begin, end, begin2, end2)
    auto it2 = std::find_first_of(s1.begin(), s1.end(), v1.begin(), v1.end());
    if (it2 != s1.end()) {
        std::cout << "find first element: " << *it2 << std::endl;    //  find first element: 27
    }

    //find_end(begin, end, begin2, end2)    // 
    auto it3 = std::find_end(s1.begin(), s1.end(), v1.begin(), v1.end());
     if (it3 != s1.end()) {
        std::cout << "find end: " << *it3 << std::endl;    //  
    }

    //find_if(begin. end, func)
    auto it4 = std::find_if(s1.begin(), s1.end(), []( const int& a) {
        return a % 2 == 0;
    });
    if (it4 != s1.end()) {
        std::cout << "it4: " << *it4 << std::endl;  //  34
    }
    
    //  search(begin, end, begin2, end2)
    auto it5 = std::search(v1.begin(), v1.end(), v2.begin() +1, v2.begin()+4);
    if (it5 != v1.end()) {
        std::cout << "it5: " << *it5 << std::endl;  //  65
        std::cout << it5 - v1.begin() << std::endl; //  1
    }

    //search_n(begin, end, n, value)
    auto it6 = search_n(v2.begin(), v2.end(), 2, 88);
    if(it6 != v2.end()) {
        std::cout << it6 - v2.begin() << std::endl; //  7
    }

    //count(begin, end, value)
    std::cout << std::count(s1.begin(), s1.end(), 34) << std::endl; //  2

    //lower_bound(begin, end, value)
    auto it7 = std::lower_bound(v2.begin(), v2.end(), 88);
    if (it7 != v1.end()) {
        std::cout << "it5: " << *it7 << std::endl;  //  88
        std::cout << it7 - v2.begin() << std::endl; //  7
    }

    //upper_bound(begin, end, value)
    auto it8 = std::upper_bound(v1.begin(), v1.end(), 32);
    if (it8 != v1.end()) {
        std::cout << *it8 << std::endl; //  34
        std::cout << it8 - v1.begin() << std::endl; //  5
    }
    //count_if(begin, end, func)
    std::cout << std::count_if(v1.begin(), v1.end(), [](int a) {
        return a % 2 == 0;
    }) << std::endl;            //  4

    //copy(begin, end, begin2)
    std::vector<int> v3;
    v3.resize(10);  //  need alloc memory here, otherwise core dump 
     print_vector(v2);  //  22 65 67 42 27 34 47 88 88 32 
    std::copy(v2.begin(), v2.begin() + 4, v3.begin() ); 
    print_vector(v3);   //  22 65 67 42 0 0 0 0 0 0 

    //transform(begin, end, begin2, func)
    v3.clear();
    v3.resize(10);  //here need alloc memory again.
    std::transform(v2.begin(), v2.end(), v3.begin(), [](int a) {
        return a * a;
    });
    print_vector(v3);   // 484 4225 4489 1764 729 1156 2209 7744 7744 1024

    //replace(begin, end, value1, value2)
    std::replace(v3.begin(), v3.end(),  7744, 88);
    print_stl(v3);  //  484 4225 4489 1764 729 1156 2209 88 88 1024

    //fill(begin, end, value)
    std::fill(v3.begin(), v3.begin() + 3, 12);
    print_stl(v3);  //  12 12 12 1764 729 1156 2209 88 88 1024


    //fill_n(begin, n, value)
    std::fill_n(v3.begin(), 2, 10);
    print_stl(v3);  // 10 10 12 1764 729 1156 2209 88 88 1024

    //generate(begin, n, rand)
    v3.clear();
    v3.resize(15);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 100);
    std::generate(v3.begin(), v3.end(), [&distrib, &gen] () {
        return distrib(gen);
    });
    print_stl(v3);  //  99 36 93 17 95 47 42 31 80 18 75 78 51 8 79 


    //remove(begin, end)
    v3 = {99, 36, 93, 17, 95, 47, 42, 31, 80, 18, 75, 78, 51, 8, 79 };
    std::remove(v3.begin() + 2, v3.begin() + 4, 93);
    print_stl(v3);  //  99 36 17 17 95 47 42 31 80 18 75 78 51 8 79

    //unique(begin, end)
    print_stl(v2);  //  22 65 67 42 27 34 47 88 88 32
    std::unique(v2.begin(), v2.end());
    print_stl(v2);  //  22 65 67 42 27 34 47 88 32 32

    //remove_if(begin, end, func)
    auto it10 = std::remove_if(v2.begin(), v2.end(), [](int a) {
        return a % 2 == 1;
    }); //  here need get it10 to confrim removed data.
    print_stl(v2);  //  22 42 34 88 32 32 47 88 32 32   
    std::cout << v2.size() << std::endl;    //  10
    std::cout << *it10 << std::endl;    //  47

    //replace_if(begin, end, func, value2)
    std::replace_if(v2.begin(), v2.end(), [](int a) {
        return a == 32;
    }, 9);
    print_stl(v2);  //  22 42 34 88 9 9 47 88 9 9

    //remove_copy(begin, end, dest)
    std::remove_copy(v2.begin(), v2.begin() + 6, v2.end() -2, 42);
    print_stl(v2);  //  22 42 34 88 9 9 47 88 22 34

    //remove_copy_if(begin, end, dest, func)
    std::vector<int> v4;
    v4.resize(16);
    v4.reserve(10);
    std::remove_copy_if(v2.begin(), v2.end(), v4.begin(), [](int a) {
        return a % 2 == 0;
    });
    std::cout << v4.size() << std::endl;
    print_stl(v4);  //  9 9 47 0 0 0 0 0 0 0 0 0 0 0 0 0

    //replace_copy(begin, end, dest, value1, value2)
    std::vector<int> v5;
    v5.resize(11);
    std::replace_copy(v2.begin(), v2.end(), v5.begin(), 9, 10);
    print_stl(v5);  //  22 42 34 88 10 10 47 88 22 34 0 0 0 0 0 0

    //replace_copy_if(begin, end, dest, func, value2)
    v5.clear();
    v5.resize(12);
    std::replace_copy_if(v2.begin(), v2.end(), v5.begin(), [](int a) {
        return a % 2 == 0;
    }, 25);
    print_stl(v2);  //  22 42 34 88 9 9 47 88 22 34
    print_stl(v5);  //  25 25 25 25 9 9 47 25 25 25 0 0


    //sort(begin, end)
    std::sort(v2.begin(), v2.end());
    print_stl(v2);  //  9 9 22 22 34 34 42 47 88 88

    //stable_sort(begin, end, func)
    std::stable_sort(v5.begin(), v5.end());
    print_stl(v5);  //  0 0 9 9 25 25 25 25 25 25 25 47

    //partial_sort(begin, mid, end)
    v2.clear(); v2.resize(6);
    v2 = {2, 8, 3, 6, 7, 9};
    std::partial_sort(v2.begin(), v2.begin() + 3, v2.end());
    print_stl(v2);  //  2 3 6 8 7 9

    //random_shuffle(begin, end)
    std::shuffle(v2.begin(), v2.end(), gen);
    print_stl(v2);  //  7 8 3 6 2 9


    //reverse(begin, end)
    std::reverse(v2.begin(), v2.end());
    print_stl(v2);  //  9 2 6 3 8 7

    //rotate(begin, mid, end)
    std::rotate(v2.begin(), v2.begin() +3, v2.end());
    print_stl(v2);  //  3 8 7 9 2 6

    //merge(begin, end, begin2, end2, nbegin)
    std::vector<int> v6;
    v6.resize(20);
    std::merge(v2.begin(), v2.end(), v5.begin(), v5.end(), v6.begin());
    print_stl(v6);  //  0 0 3 8 7 9 2 6 9 9 25 25 25 25 25 25 25 47 0 0

    //reverse_copy(begin, end, dest)
    v6.clear(); v6.resize(12);
    std::reverse_copy(v2.begin(),v2.end(), v6.begin());
    print_stl(v6);  //  6 2 9 7 8 3 0 0 0 0 0 0

    //rotate_copy(begin, mid, end, dest)
    v6.clear(); v6.resize(12);
    std::rotate_copy(v2.begin(), v2.begin() + 3, v2.end(), v6.begin());
    print_stl(v6);  //  9 2 6 3 8 7 0 0 0 0 0 0

    //equal(begin, end, begin2, end2)
    bool is_equal = std::equal(v2.begin(), v2.end(), v6.begin(), v6.begin() + 4);
    std::cout << is_equal << std::endl; //  0

    //includes(begin, end, begin2, end2)
    std::vector<int> v7 = {9,2, 6, 3, 8};
    std::cout << std::includes(v6.begin(), v6.end(), v7.begin(), v7.end()) << std::endl;    //  1

    //max_element(begin, end)
    auto it11 = std::max_element(v6.begin(), v6.end());
    std::cout << *it11 << std::endl;    //  9

    //min_element(begin, end)
    auto it12 = std::min_element(v6.begin(), v6.end());
    std::cout << *it12 << std::endl;    //  0

    //mismatch(begin, end, begin2, end2)
    auto it13 = std::mismatch(v6.begin(), v6.end(), v7.begin(), v7.end());
    std::cout << *it13.first  << std::endl; //  7
    std::cout << *it13.second  << std::endl;    //25


}


void test_stl() {

    //test_vecotr();
    //test_list();
    //test_map();
    //test_multimap();
    //test_unordered_map();
    //test_set();
    //test_multiset();
    //test_forward_list();
    //test_array();
    //test_stack();
    //test_deque();
    //test_queue();
    //test_priority_queue();
    test_stl_algorithm();
}