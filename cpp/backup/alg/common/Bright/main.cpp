#include<iostream>
#include"public.h"
#include<ctime>
#include<fstream>




using namespace std;

#define MAX_NUM 20
#define TEXT_BEFORE_SORT ("./before.txt")
#define TEXT_END_SORT ("./End.txt")



void Print(int *arr, int len);
bool WriteMyFile(string str, int*pData, int nLen);


bool TestQuickSort();


int main(int argc, char* argv[])
{
	//_CrtSetBreakAlloc(157);
	TestQuickSort();


	
	
	_CrtDumpMemoryLeaks();
	return 0;
}

bool WriteMyFile(string str, int*pData, int nLen)
{
	fstream	file(str.c_str(), ios::in | ios::out |ios::app);
	if(!file)
	{
		return false;
	}

	for(int i = 0; i < nLen; ++i)
	{
		file << *pData++  << "  ";
	}
	file.close();
	return true;
}

bool TestQuickSort()
{
	CCommonAlg* pFun = new CCommonAlg();

	int* pData = new int[MAX_NUM];
	for(int i = 0; i < MAX_NUM; ++i)
	{
		pData[i] = rand() % MAX_NUM;
		
	}
	if(!WriteMyFile(TEXT_BEFORE_SORT, pData, MAX_NUM))
	{
		cout << "Open file failed!Path:" << TEXT_BEFORE_SORT << endl;
		return false;
	}
	//pFun->Print(arr, MAX_NUM);
	double start,end;
	start = clock();
	pFun->QuickSort(pData, MAX_NUM, 1);
	end = clock();
	printf("Time pass:%f\n", (end - start) / CLOCKS_PER_SEC);
	//pFun->Print(pData, MAX_NUM);

	if(!WriteMyFile(TEXT_END_SORT, pData, MAX_NUM))
	{
		cout << "Open file failed!Path:" << TEXT_END_SORT << endl;
		return false;
	}
	

	
	if(pFun)
	{
		delete pFun;
	}
	
	if(pData)
	{
		delete[] pData;
	}
	
	return true;
}