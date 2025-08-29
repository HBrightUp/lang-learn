#include <iostream>
#include<vector>
#include<cstring>
#include<sstream>
#include<iomanip>
#include<algorithm>
#include <functional>
#include <crtdbg.h>


using namespace std;

/******************** Begin:Test 2 **********************/
template<typename T>
void PrettyFormat(T value, string& str)
{
	ostringstream temp;
	temp << setw(4) << value;
	str= temp.str();
}
/******************** End:Test 2 **********************/
/******************** Begin:Test 3 **********************/
class Employee
	{
	public:
		void DoSomething()
		{
			cout << ++ m_nCal << endl;
		}

	private:
		static int m_nCal;
	};

int Employee::m_nCal = 100;

/******************** End:Test 3 **********************/
/********************Begin: Test 4 **********************/
//总结： 可以通过四种方法破坏类成员的private性质
//1.添加友元函数；2.利用define宏; 3.利用结构相同的欺骗类；4.函数模板(这里没有尝试)

#if 0	//
#include"TestFour.h"
void BadFunFour(CTestFour& four)
{
	four.m_nPrivate = 100;
}


class CTestFourBad
{
public:
	friend void ::BadFunFour(CTestFour& four);
};
#endif
#if 0
#include"TestFour.h"

void FunFour(CTestFour& x)
{
	(reinterpret_cast<CTestFourPianZi&>(x)).m_notPrivate = 99;
}

#endif

/******************** End:Test 4 **********************/

/******************** Begin:Test 5 **********************/



#include "TestFive.h"


/******************** End:Test 5 **********************/

void TestOne();
void TestTwo();
void Test_3();
void Test_4();
void Test_5();

int main(int argc, char* argv[])
{
	
	//_CrtSetBreakAlloc(121);
	Test_5();
	

	_CrtDumpMemoryLeaks();
	return 0;
}


void TestOne()
{
	vector<int> v;

	v.resize(10);
	v[0] = 1;
	v[1] = 2;
	v[2] = 3;
	v[4] = 8;
	v[9] =100;
	copy(v.begin(), v.end(), ostream_iterator<int>(cout, "  "));
	
}

void TestTwo()
{
	int i = 1900;
	string strPrint;

	PrettyFormat(i, strPrint);
	cout << strPrint.data() << endl;
	
	double d = 123.362954;
	PrettyFormat(d, strPrint);
	cout << strPrint.c_str() << endl;

}


void Test_3()
{
	vector<Employee*> emps;

	int ncapacity = emps.capacity();
	emps.resize(10);			// need resize,or error!!
	for(int i = 0; i < 10; ++i)
	{
		emps[i] = new Employee();
	}

	// other place,if it is a object ,use mem_fun_ref().
	for_each(emps.begin(), emps.end(), mem_fun(&Employee::DoSomething));

	cout << ncapacity << endl;
	
}
void Test_4()
{

#if 0	//使用宏可以欺骗
	#define private public
	#include"TestFour.h"
	CTestFour Test;
	Test.m_nPrivate = 10000;
#endif
	
#if 0
	CTestFour four;
	four.SetValue(10);
	int nRet = four.GetValue();
	FunFour(four);
	 nRet = four.GetValue();
#endif

}
void Test_5()
{
#define My_ARR 10

	void* p = ::operator new(My_ARR * sizeof(CTestFive));
	new (p)CTestFive[My_ARR];	//一次new十个对象
	
	

	CTestFive* pArr = NULL;
	
	for(int i = 0; i < My_ARR;++i)
	{
		pArr = (CTestFive*)p + i;
		pArr->m_nSun = i + 1;
		pArr->PrintData();
	}
	
	for(int i = 0; i < My_ARR;++i)
	{
		pArr = (CTestFive*)p + i;
		
		pArr->PrintData();
		pArr->~CTestFive();
	}

	
	
		delete[] p;

}