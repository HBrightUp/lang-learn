#include<iostream>
#include"type_traits"
#include"../include/enum_class.h"


std::ostream& operator << (std::ostream& os, altitude al)
{
	return os << "char: " << static_cast<char>(al);
}


void enumclass_example() {

    altitude a = altitude::low;
    std::cout << a << std::endl;    //  char: l

    Color c = Color::BLUE;
    if (c != Color::RED) {
        std::cout << "color: " << static_cast<int>(c) << std::endl; // color: 1
    }

    std::cout << std::boolalpha;
    std::cout << "Is enum: " << std::is_enum_v<Fruit> << std::endl; // true
    std::cout << "Is enum: " << std::is_enum_v<Color> << std::endl; //true
    std::cout << "Is scope enum: " << std::is_scoped_enum_v<Fruit> << std::endl;    // false
    std::cout << "Is scope enum: " << std::is_scoped_enum_v<Color> << std::endl;    // true
    std::cout << "Is scope enum: " << std::is_scoped_enum_v<int> << std::endl;      // false
}