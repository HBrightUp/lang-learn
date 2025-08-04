#pragma once
#include "./animal.h"


class Cat : public Animal {

    public:
        Cat() = default;
        virtual void sound() const ;
        virtual ~Cat();
};