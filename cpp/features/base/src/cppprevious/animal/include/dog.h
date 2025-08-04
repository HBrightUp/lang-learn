#pragma once
#include "./animal.h"


class Dog : public Animal {

    public:
        Dog() =default;
        virtual void sound() const ;
        virtual ~Dog();
};