#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<map>

using namespace std;

#define MAX_VECTOR_NUM 20

typedef struct st_vector_data
{
	int nPos;
	string strData;
}VECTORDATA,*PVECTORDATA;


void Print(vector<VECTORDATA> &vc);
ostream& operator << (ostream& out,  vector<VECTORDATA>& vc);
bool operator == (const VECTORDATA &A, const VECTORDATA &B);
bool operator > (const VECTORDATA &A, const VECTORDATA &B);
bool operator < (const VECTORDATA &A, const VECTORDATA &B);

//=， ->， operator[ ]， operator(),不能进行全局重载
/*VECTORDATA operator[] (unsigned int nPos);*/

class CRmvB
{
public:
	CRmvB(const string& str):m_strData(str) {}
	bool operator () (const vector<VECTORDATA>::value_type& vc)
	{
		return vc.strData > m_strData;;
	}

private:
	string m_strData;
};

void TestVector();

int main()
{
	VECTORDATA vcData;
	vcData.nPos = -1;
	vcData.strData = "null";
	vector<VECTORDATA> vc_A(10, vcData);
	cout << vc_A;
	
	vc_A.clear();
	
	char buf[32];
	memset(buf, 0, sizeof(buf));
	string strInit;
	int nRand;
	for(int i = 0; i < MAX_VECTOR_NUM; ++i)
	{
		nRand = rand() % MAX_VECTOR_NUM;
		sprintf(buf, "%d", nRand);
		strInit = "A";

		strInit += buf;
		vcData.nPos = nRand;
		vcData.strData = strInit;
		vc_A.push_back(vcData);
	}
	cout << vc_A;

	cout<< "cpacity:" << vc_A.capacity() << ",max_size:" << vc_A.max_size() << ",size:" << vc_A.size()  << endl;


	//delete the elem double 
	vector<VECTORDATA>::iterator iter;
	for(iter = vc_A.begin(); iter != vc_A.end(); )
	{
		if(iter->nPos % 2 == 0)
		{
			iter = vc_A.erase(iter);
		}
		else
		{
			++iter;
		}
		
	}
	cout << vc_A ;

	vector<VECTORDATA> vc_B;
	vc_B.assign(vc_A.begin(), vc_A.end());

	cout << vc_B ;
	
	//delete repeate data
	sort(vc_B.begin(), vc_B.end());
	vc_B.erase(unique(vc_B.begin(), vc_B.end()), vc_B.end());
	cout << vc_B ;

	vc_B[4] = vcData;
	cout << vc_B ;
	
	strInit = "A8";
	iter = find_if(vc_B.begin(), vc_B.end(), CRmvB(strInit));
	if(iter == vc_B.end())
	{
		cout << "find data failed!" << endl;
	}
	else
	{
		vc_B.erase(iter, vc_B.end());
	}
	cout << vc_B ;


	return 0;
}

ostream& operator << (ostream& out,  vector<VECTORDATA>& vc)
{
	for(vector<VECTORDATA>::iterator iter = vc.begin(); iter != vc.end(); ++iter)
	{
		if(iter->nPos == -1)
		{
			break;
		}

		cout <<iter->nPos <<  "," << iter->strData.data() << "  ";
	}
	cout << endl;

	return out;
}

bool operator == (const VECTORDATA &A, const VECTORDATA &B)
{
	return A.nPos == B.nPos;
}

bool operator > (const VECTORDATA &A, const VECTORDATA &B)
{
	return A.nPos > B.nPos;
}

bool operator < (const VECTORDATA &A, const VECTORDATA &B)
{
	return A.nPos < B.nPos;
}







