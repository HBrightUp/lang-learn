#include<iostream>
#include<type_traits>
#include<functional>
#include<utility>
#include"../include/type_traits.h"



void func() {}
class A{};
class B {
    public:
        virtual void send_message(std::string msg) = 0;
};

enum class C:int{ a, b, c};
enum D {};

void call_type_traits() {

    int a = 3;

    std::cout << std::is_integral<decltype(a)>::value << std::endl; //  1
    std::cout << std::is_integral<float>::value << std::endl;   //  0
    std::cout << std::is_floating_point<double>::value << std::endl;  ///   1
    std::cout << std::is_pointer<double *>::value << std::endl;  // 1
    std::cout << std::is_reference<unsigned int>::value << std::endl;   //  0
    
    std::cout << "class type." << std::endl;
    std::cout << std::is_abstract<A>::value << std::endl;   //  0 
    std::cout << std::is_abstract<B>::value << std::endl;   //  1   
    std::cout << std::is_class<A>::value << std::endl;  //  1
    std::cout << std::is_enum<B>::value << std::endl;   //  0

    std::cout << std::is_integral_v<int*> << std::endl; //  0
    std::cout << std::is_scoped_enum_v<C> << std::endl; //  1
    std::cout << std::is_scoped_enum_v<D> << std::endl; //  0

    std::cout << " same type" << std::endl;
    std::cout  << std::boolalpha;
    std::cout << std::is_same_v<char, decltype(std::to_underlying(C::a))> << std::endl; //  false
    std::cout << std::is_same_v<int, decltype(std::to_underlying(C::b))> << std::endl; //  true

    std::cout << "bounded array" << std::endl;
    std::cout << std::is_bounded_array_v<A> << '\n';    //  false
    std::cout << std::is_bounded_array_v<A[]> << '\n';  //  false
    std::cout << std::is_bounded_array_v<A[3]> << '\n'; //  true
    std::cout << std::is_bounded_array_v<float> << '\n';    //  false
    std::cout << std::is_bounded_array_v<int> << '\n';  //  false
    std::cout << std::is_bounded_array_v<int[]> << '\n';    //  false
    std::cout << std::is_bounded_array_v<int[3]> << '\n';   //  true
}