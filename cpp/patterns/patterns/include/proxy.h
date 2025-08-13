#pragma once

#include<iostream>

class IProxy
{
public:
    virtual void Run() = 0;
};

class RealSubject :public IProxy
{
public:
    virtual void Run()
    {
        std::cout << "RealSubject run" << std::endl;
    }
};

class RealProxy :public IProxy
{
public:
    RealProxy(std::string userName, std::string password) :
        m_userName(userName),
        m_password(password),
        m_real{ new RealSubject() }
    {
        //...
    }
    virtual void Run()
    {
        std::cout << "Proxy run" << std::endl;
        if (checkUserNameAndPassword())
        {
            m_real->Run();
        }
        else
        {
            std::cout << "system check error" << std::endl;
        }
    }
    ~RealProxy()
    {
        if (m_real != nullptr)
        {
            delete m_real;
        }
    }
private:
    bool checkUserNameAndPassword()
    {
        return true;
    }
public:
    RealSubject* m_real;
    std::string m_userName;
    std::string m_password;
};

