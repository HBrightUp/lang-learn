#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<algorithm>

using namespace std;

#define MAX_SIZE 10

typedef struct _map_key
{
	unsigned int nNum;
	char strName[32];
}MAPKEY,*PMAPKEY;

typedef struct _map_value
{
	bool bMale;
	int nScore;
	string strSubject;
	
}MAPVALUE,*PMAPVALUE;
typedef pair<MAPKEY, MAPVALUE> MYPAIR;
typedef map<MAPKEY, MAPVALUE> MYMAP;

ostream& operator << (ostream& out, map<MAPKEY, MAPVALUE>& m);
ostream& operator << (ostream& out, vector<MYPAIR>& vc);
bool operator < (const MAPKEY k1, const MAPKEY k2);


bool CmpByScore(const MYPAIR& lhs, const MYPAIR& rhs);


class CMyMapVal
{
public:
	CMyMapVal(const MAPVALUE& val):m_mapVal(val){}
	bool operator () (const MYMAP::value_type& pair)
	{
		return (pair.second.bMale == m_mapVal.bMale) && (pair.second.nScore == m_mapVal.nScore) && (pair.second.strSubject == m_mapVal.strSubject);
	}

private:
	MAPVALUE m_mapVal;

};



int main(int argc, char* argv[])
{
	map<MAPKEY, MAPVALUE> m;
	
	MAPKEY key;
	string strBase = "Go";
	char buf[32];
	memset(buf, 0, sizeof(buf));
	MAPVALUE val;

	for(int i = 0; i < MAX_SIZE; ++i)
	{
		key.nNum =100 + rand() % MAX_SIZE;
		sprintf(buf,"%d",key.nNum);
		strBase = "Go";
		strBase += buf;
		memcpy(key.strName, strBase.data(), strBase.length() + 1);

		val.bMale = static_cast<bool>(i % 2);
		val.nScore = 60 + rand() % 40;
		val.strSubject = "English";

		m[key] = val;
	}
	cout << m << endl;

	// sort by score
	vector<MYPAIR> vc_score(m.begin(), m.end());
	sort(vc_score.begin(), vc_score.end(),CmpByScore);
	cout << vc_score << endl;
	
	map<MAPKEY, MAPVALUE> n(m);
	cout << n << endl;

	key.nNum = 105;
	strBase = "Go105";
	memcpy(key.strName, strBase.c_str(), strBase.length() + 1);

	val = n[key];
	
	map<MAPKEY, MAPVALUE>::iterator iter;
	//iter = find(n.begin(), n.begin(), key);   // error,why?
	
	
	iter = n.find(key);
	if(iter == n.end())
	{
		cout << "find data! " << endl;
	}
	else
	{
		//iter = n.erase(iter);		//  right
	}

	cout << n << endl;
	iter = find_if(n.begin(), n.end(), CMyMapVal(val));		//	right

	return 0;
}

ostream& operator << (ostream& out,  map<MAPKEY, MAPVALUE>& m)
{
	map<MAPKEY, MAPVALUE>::iterator iter;
	for(iter = m.begin(); iter != m.end(); ++iter)
	{
		out << iter->first.nNum << "," << iter->first.strName << ": " << iter->second.bMale << "," <<
			iter->second.nScore << "," << iter->second.strSubject << endl;
	}
	return out;
}

bool operator < (const MAPKEY k1, const MAPKEY k2)
{
	return k1.nNum < k2.nNum;
}

bool CmpByScore(const MYPAIR& lhs, const MYPAIR& rhs)
{
	return lhs.second.nScore < rhs.second.nScore;
}

ostream& operator << (ostream& out, vector<MYPAIR>& vc)
{
	vector<MYPAIR>::iterator iter;
	for(iter = vc.begin(); iter != vc.end(); ++iter)
	{
		out << iter->first.nNum << "," << iter->first.strName << ": " << iter->second.bMale << "," <<
			iter->second.nScore << "," << iter->second.strSubject << endl;
	}

	return out;
}


