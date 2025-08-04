#pragma once



class Animal {

    public:
        Animal() = default;
        virtual void sound() const ;
        virtual ~Animal();
};