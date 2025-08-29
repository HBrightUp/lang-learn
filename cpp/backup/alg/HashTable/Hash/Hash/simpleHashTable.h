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
    Entry& operator[](KeyType key);//�����±���������Ҳ���key��Ӧ��Entryʱ������Entry(key,0)  
    int size();  
    void display();  
  
protected:  
    int hashFunction(KeyType key);//����ֵӳ�䵽��Ӧ��ַ  
    void rehash();//����hashTable��С  
    bool find(const KeyType& k);//����ֵ����  
    int nextPrime();//p(n) = n^2 - n + 41, n<41, p<1681  
  
private:  
    Entry *_pTable;  
    int _pos;//��ǰ����Ԫ�ص�λ��  
    int _size;  
    int _capacity;  
    int primeIndex;  
};  