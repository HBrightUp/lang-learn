#include<iostream>
#include<string>
#include<set>

using namespace std;

#define MAX_BUFFERSIZE 64
#define MAX_NUM 10

typedef struct  _set_data
{
	int nPos;
	char strData[MAX_BUFFERSIZE];
}MYSETDATA, *PMYSETDATA;

typedef set<MYSETDATA> ST;


void InitData(ST& m);
int main()
{
	ST m;

	
	



	return 0;
}

void InitData(ST& m)
{
	for(int i = 0; i< MAX_NUM; ++i)
	{

	}
}