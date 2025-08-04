#include"./include/object.h"
#include "./animal/include/animal.h"
#include "./animal/include/cat.h"
#include "./animal/include/dog.h"

void inherit() {

    Animal* p1 = new Animal();
    p1->sound();
    delete p1;

    p1 = new Dog();
    p1->sound();
    delete p1;

    p1 = new Cat();
    p1->sound();
    delete p1;
}