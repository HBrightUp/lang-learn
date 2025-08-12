#include <iostream>

class Singleton
{
    public:
        ~Singleton(){
            std::cout<<"Destructor called!"<<std::endl;
        }
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        static Singleton& get_instance() {
            static Singleton instance;
            return instance;
        }
    private:
        Singleton(){
            std::cout<<"Constructor called!"<<std::endl;
        }
};

