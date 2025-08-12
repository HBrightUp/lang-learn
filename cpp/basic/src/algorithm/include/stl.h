#pragma once
#include<ostream>






struct Person {
    unsigned age;
    std::string name;
    
    std::ostream& operator<<(std::ostream& os) {
        os << "age: " << age << ", name: " << name << std::endl;
        return os;
    }

    bool operator==(const Person& other) const {
        return age == other.age && name == other.name;
    }

   
};

template <>
struct std::hash<Person> {
    size_t operator()(const Person& p) const {
        return std::hash<unsigned>()(p.age) ^ std::hash<std::string>()(p.name);
    }
};






void test_stl();

