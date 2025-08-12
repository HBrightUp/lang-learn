#include<iostream>
#include"../include/convert.h"



void static_convert() {

    int a = 100;
    char c = a;
    std::cout << "c: " << c << std::endl;   //  d

    double d = 3.1323323532;
    float f = d;
    std::cout << "f: " << f << std::endl;   //  f: 3.13233
}

void const_convert() {

    const  int max = 323;
    int* a  = const_cast<int*>(&max);   //  error use
    *a = 90;
    std::cout << "max: " << max << std::endl;   //323

    int min = 45;
    const int* b  = &min;

    int* c = const_cast<int*> (b);  //  only use  * or & but not variable self.
    *c = 90;
    std::cout << "min: " << min <<  std::endl;  //  90
    
}

void dynamic_convert() {

    Base* pb = new Derived();
    Derived* pd = dynamic_cast<Derived*> (pb);

    if (pd) {
        pd->print();
        pd->derivedMethod();
    } else {
        std::cout << " dynamic failed!" << std::endl;
    }

    // Derived class
    // Derived method
    // Derive destructor
    // Base destruct

    delete pb;
}

void reinterpret_convert() {
    int num = 0x00636261;
	int * pnum = &num;
	char * pstr = reinterpret_cast<char *>(pnum);
    unsigned* puns =  reinterpret_cast<unsigned*>(pnum);
	std::cout << "pnum:: " << pnum << std::endl;
	std::cout << "pstr:: " << static_cast<void *>(pstr) << std::endl;
	std::cout << "pnum content: " << std::hex << *pnum << std::endl;
	std::cout << "pstr content: " << pstr << std::endl;
    std::cout << "puns content: " << std::hex << *puns << std::endl;

    // pnum:: 0x7ffd7f92cad4
    // pstr:: 0x7ffd7f92cad4
    // pnum content: 636261
    // pstr content: abc
    // puns content: 636261
}

void call_convert() {
    std::cout << " Call convert." << std::endl;
    //static_convert();
    //const_convert();
    //dynamic_convert();
    reinterpret_convert();
}