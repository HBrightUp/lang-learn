#pragma once
#include<iostream>
#include <string>
#include <map>

class Font
{
public:
    Font(const std::string &k): key(k) {};
    void show() { std::cout << "key: " << key << std::endl;}

private:
    std::string key;
};

class FlyWeightFactory
{
public:
    Font *GetFont(const std::string &key) //一个key只有一个对象
    {
        std::map<std::string, Font *>::iterator iter = fontPool.find(key);
        if (iter != fontPool.end())
        {
            return fontPool[key];
        }
        else
        {
            Font *font = new Font(key);
            fontPool[key] = font;
            return font;
        }
    }
    void clear()
    {
        //...
    }

private:
    std::map<std::string, Font *> fontPool;
};