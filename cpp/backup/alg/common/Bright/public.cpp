#include"public.h"
#include<iostream>

using namespace std;

template<class T> 
void SWAP(T &a,T &b) 
{ 
	T t; 
	t = a; 
	a = b; 
	b =t; 
} 


// 快速排序算法的递归算法   
void CCommonAlg::quicksortByCycle( int*arr, int beg, int end )  
{  
    int left, right;  
    int val;  
  
    if ( beg >= end )  
    {  
        return;  
    }  
  
    left = beg;  
    right = end+1;  
    val = arr[beg];  
  
    for ( ;; )  
    {  
        while( ++left <= end && arr[left] < val );  
        while( arr[--right] > val );  
        if ( left > right )  
            break;  
        SWAP( arr[left], arr[right]);  
    }  
    SWAP( arr[beg], arr[right] );  
  
    quicksortByCycle(arr, beg, right - 1);  
    quicksortByCycle(arr, right + 1, end);  
}  
  
// 快速排序算法的非递归算法  
void CCommonAlg::quicksortNoCycle( int* A, int n )  
{  
    struct sbe  
    {  
        int beg;  
        int end;  
    };  
    sbe* arr = new sbe[n];  
    int beg, end, left, right, val, m;  
    m = 0;  
    arr[0].beg = 0;  
    arr[0].end = n-1;  
    while(1) 
    {  
        if ( m < 0 )  
            break;  
        beg = arr[m].beg;  
        end = arr[m--].end;  
        if ( beg >= end )  
            continue;     
        val = A[beg];  
        left = beg;  
        right = end+1;  
        while(1)  
        {  
            while( ++left <= end && A[left] < val );  
            while( A[--right] > val ); 
            if ( left > right )  
                break;  
			if(A[left] != A[right])
			{
				Print(A,n);
				SWAP( A[left], A[right] ); 
			}	
        } 
		if(A[right] != A[beg])
		{
			Print(A,n);
			SWAP( A[right], A[beg] );  
		}
        arr[++m].beg = beg;  
        arr[m].end   = right-1;  
        arr[++m].beg = right+1;  
        arr[m].end   = end;  
    }  
	Print(A,n);
    delete [] arr;  
}  
  
// 快速排序驱动程序  
void CCommonAlg::QuickSort( int* arr, int n, int method)  
{  
    if ( method == 0 )  
    {  
        quicksortByCycle( arr, 0, n-1);  
    }  
    else  
    {  
        quicksortNoCycle( arr, n );  
    }  
}  

void CCommonAlg::Print(int *arr, int len)
{
	
	for(int i = 0; i < len; ++i)
	{
		cout << arr[i] << "  ";
	}

	cout << endl;
}