#ifndef _PUBLIC_H_
#define _PUBLIC_H_


class CCommonAlg
{
public:
	CCommonAlg() {};
	~CCommonAlg() {};

public:
	void CCommonAlg::quicksortByCycle( int* arr, int beg, int end);
	void CCommonAlg::quicksortNoCycle( int* arr, int nLen)  ;
	
	//采用方法，0 - 递归， 1 - 非递归 
	void CCommonAlg::QuickSort( int* arr, int nLen, int method = 0);  
 
	void Print(int *arr, int len);
};









#endif





