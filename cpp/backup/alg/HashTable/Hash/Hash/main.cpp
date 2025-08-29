#include <iostream>  
#include "simpleHashTable.h"  
  
using namespace std;  
 
#define MAX_LEN 1000000
int main()  
{  
    hashTable *pTable = new hashTable;  

#if 1

	for(int i = 0; i < MAX_LEN;++i)
	{
		pTable->insert(Entry(i,i +100)); 
	}

	
	cout << "insert data finished!" << endl;  
	Entry en(500000,500000);
	pTable->find(en);

#endif

#if 0
    cout << "insert Entry(1,11)..." << endl;  
    pTable->insert(Entry(1,11));  
    pTable->display();  
    cout << "insert Entry(2,22)..." << endl;  
    pTable->insert(Entry(2,22));  
    pTable->display();  
    cout << "insert Entry(3,33)..." << endl;  
    pTable->insert(Entry(3,33));  
    pTable->display();  
    cout << "insert Entry(4,44)..." << endl;  
    //pTable->insert(Entry(4,44));  
    (*pTable)[4]._value = 44;//下标操作，返回值充当左值  
    pTable->display();  
  
    cout << endl << "delete Entry(1,11)..." << endl;  
    pTable->remove(Entry(1,11));  
    pTable->display();  
    cout << "delete Entry(2,22)..." << endl;  
    pTable->remove(Entry(2,22));  
    pTable->display();  
    cout << "delete Entry(3,33)..." << endl;  
    pTable->remove(Entry(3,33));  
    pTable->display();  
  
    cout << "delete Entry(3,33)..." << endl;  
    pTable->remove(Entry(3,33));  
    pTable->display(); 

#endif


      
    delete pTable;  
    getchar();  
    return 0;  
}  