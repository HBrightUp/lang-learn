#include "simpleHashTable.h"  
  
hashTable::hashTable()  
{  
    _capacity = 3;//初始化hashTable容量为3,便于观察rehash过程  
    _pTable = new Entry[_capacity];  
    _size = 0;  
    primeIndex = 1;  
}  
  
//hashTable::hashTable(int tableSize)  
//{  
//  
//}  
  
hashTable::~hashTable()  
{  
    clear();  
}  
  
int hashTable::nextPrime()  
{  
    int p = std::pow(static_cast<float>(primeIndex),2) - primeIndex + 41;  
    primeIndex = primeIndex << 1;  
    if(primeIndex >= PIRME_MAX){  
        cout << "Max capacity reached. exit!" << endl;  
        return -1;  
    }  
    return p;  
}  
  
bool hashTable::find(const Entry&  e)  
{  
    return(find(e._key));  
}  
  
bool hashTable::find(const KeyType& k)  
{  
    _pos = hashFunction(k);  
    if(_pTable[_pos]._key==NULLKEY)  
        return false;  
    int lastPos = _pos;  
    while(_pTable[_pos]._key!=k){  
        if(++_pos%_capacity == lastPos)  
            return false;  
    }  
    return true;  
}  
  
bool hashTable::insert(const Entry& e)  
{  
    if((_size*1.0)/_capacity>0.75)  
        rehash();//[OK]插入操作前，需要判断hash table是否需要扩容  
    if(find(e))  
        return false;  
    _pTable[_pos] = e;  
    ++_size;  
    return true;  
}  
  
bool hashTable::remove(const Entry& e)  
{  
    if(!find(e))  
        return false;  
    _pTable[_pos]._key = NULLKEY;  
    --_size;  
    //rehash();//移除操作后，需要判断hash table是否需要缩容  
    return true;  
}  
  
void hashTable::clear()  
{  
    delete []_pTable;  
    _size = _capacity = 0;  
}  
  
Entry& hashTable::operator[](KeyType key)  
{  
    if(!find(key))  
        insert(Entry(key,0));  
    return _pTable[_pos];  
}  
  
int hashTable::size()  
{  
    return _size;  
}  
  
int hashTable::hashFunction(KeyType key)  
{  
    return key%_capacity;  
}  
  
void hashTable::display()  
{  
    cout << "capacity = " << _capacity << ", size = " << _size << endl;  
    for(int i=0; i<_capacity; i++){  
        if(_pTable[i]._key!=NULLKEY)  
            cout << "key=" << _pTable[i]._key << ",value=" << _pTable[i]._value << endl;  
    }  
}  
  
  
void hashTable::rehash()  
{     
    cout << "begin rehash..." << endl;  
    Entry *p = new Entry[_size];//用来暂存原哈希表  
    for(int i=0; i<_capacity; i++){//i<_size不对；元素散列在容量为_capacity的hashTable中  
        if(_pTable[i]._key != NULLKEY)  
            *(p+i) = _pTable[i];  
    }  
    delete []_pTable;  
    int lastSize = _size;  
    _size = 0;    
  
    _capacity = nextPrime();
	if(_capacity == -1)
	{

	}
    _pTable = new Entry[_capacity];  
    for(int i=0; i<lastSize; i++)  
        insert(*(p+i));  
    delete []p;       
}  