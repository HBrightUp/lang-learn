
#include<iostream>
#include<memory>
#include"../include/pointer.h"
#include"../include/convert.h"




void call_unique() {

    const int len = 10;
    std::unique_ptr<int> ptr(new int[len]);
    int temp = len;

    while(temp--) {
         ptr.get()[temp] = temp;

    }
   
    std::cout << "ptr: " << ptr.get()[5] << std::endl; 


    std::unique_ptr<Base> pb(new Derived());

    pb.get()->print();  //  Derived class

    pb.reset();     
    //  Derive destructor   
    //  Base destruct 

    std::cout << "leave here" << std::endl;


}


void call_share() {

    std::shared_ptr<Base> pb(new Derived());

    auto p1 = pb;

    auto p2 = pb;

    std::cout << pb.use_count() << std::endl;   //  3

    p1.reset();

    std::cout << pb.use_count() << std::endl;   //  2

    {
        auto p4 = p2;
        std::cout << pb.use_count() << std::endl;   //  3;
    }

    std::cout << pb.use_count() << std::endl;   //  2

    p2.get()->print();  //  Derived class

    std::cout << pb.unique() << std::endl;  //  0


    std::shared_ptr<Base> p3;

    pb.swap(p3);

    std::cout << pb.use_count() << std::endl; // 0
    std::cout << p3.use_count() << std::endl;   //  2


    p3.reset();
    std::cout << p2.use_count() << std::endl;   //  1
    std::cout << "leave here" << std::endl;

    //  Derive destructor
    //  Base destruct
}




void call_pointer() {
    //call_unique();
    call_share();
}