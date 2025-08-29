#pragma once  
#include <iostream>  
#include<complex>

using namespace std;  

  
typedef int KeyType;  
#define NULLKEY -1  

#define PIRME_MAX 100000000
  
struct Entry{  
    KeyType _key;  
    int     _value;  
    Entry(KeyType key=NULLKEY, int value=0):_key(key),_value(value){}  
};  
  
class hashTable{  
public:  
    hashTable();  
    //hashTable(int tableSize);  
    ~hashTable();  
  
    bool find(const Entry&  e);  
    bool insert(const Entry& e);  
    bool remove(const Entry& e);  
    void clear();  
    Entry& operator[](KeyType key);//重载下标操作；当找不到key对应的Entry时，插入Entry(key,0)  
    int size();  
    void display();  
  
protected:  
    int hashFunction(KeyType key);//将键值映射到对应地址  
    void rehash();//调整hashTable大小  
    bool find(const KeyType& k);//按键值查找  
    int nextPrime();//p(n) = n^2 - n + 41, n<41, p<1681  
  
private:  
    Entry *_pTable;  
    int _pos;//当前访问元素的位置  
    int _size;  
    int _capacity;  
    int primeIndex;  
};  