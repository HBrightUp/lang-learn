#include<iostream>

class CTestFive
{
public:
	CTestFive() {std::cout << "construct:" << ++m_nPrivate << std::endl;}
	~CTestFive() {std::cout << "~construct:" << --m_nPrivate << std::endl;}

	void PrintData() {std::cout << "PrintData:" << m_nSun << std::endl;}


private:
	 static int  m_nPrivate;
public:
	 int m_nSun;

};

 int CTestFive::m_nPrivate = 0;