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
	
	//���÷�����0 - �ݹ飬 1 - �ǵݹ� 
	void CCommonAlg::QuickSort( int* arr, int nLen, int method = 0);  
 
	void Print(int *arr, int len);
};









#endif





