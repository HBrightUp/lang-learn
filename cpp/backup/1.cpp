#include<iostream>
#include<string>
#include<algorithm>
#include<fstream>
#include<map>
#include<vector>

using namespace std;

#define FILE_PATH ("./test.txt")
#define MAX_CHAR_ONE_LINE (1024 * 2)

struct stWordInfo
{
	unsigned int nTimes;
	vector<int> vc;			// 记录出现在哪些行
};
typedef map<string, stWordInfo> mapWords;


void DealLineWords(mapWords& m, const string& strWords, unsigned int nLine);
bool GetWordsPos(string str , unsigned int& beg, unsigned int& end);
ostream& operator << (ostream& out, mapWords& m);

int main(int argc, char* argv[])
{
	map<string, stWordInfo> m;
	
	fstream file;
	file.open(FILE_PATH, ios::in);
	if(!file.is_open())
	{
		cout << "Open file failed!" << endl;
		return 0;
	}

	string strLine;
	unsigned int nLine = 0;

	while(getline(file, strLine))
	{
		DealLineWords(m, strLine, ++nLine);
	}

	file.close();
	return 0;
}

void DealLineWords(mapWords& m, const string& strWords, unsigned int nLine)
{
	unsigned int nLen = strWords.length();
	unsigned int beg,end;
	string strSub;
	stWordInfo st;
	st.nTimes = 1;
	st.vc.push_back(nLine);
	
	beg = end = 0;

	while(beg < nLen)
	{
		if(!GetWordsPos(strWords, beg, end))
		{
			break;
		}
		
		strSub = strWords.substr(beg, end-beg);

		mapWords::iterator iter = m.find(strSub);
		if(iter != m.end())
		{
			++iter->second.nTimes;
			iter->second.vc.push_back(nLine);
		}
		else
		{
			m[strSub] = st;
		}
		
		//cout << m;
		beg = end;
	}
}

bool GetWordsPos(string str , unsigned int& beg, unsigned int& end)
{
	char ch;
	
	//  65~90,97~122     
	while(1)
	{
		ch = str.at(beg);
		if((ch < 'A' || 'z' > 122) || ( 'Z' < ch && ch < 'a'))
		{
			if(++beg < str.length())
			{
				continue;
			}
			else
			{
				return false;
			}
		}
		else
		{
			break;
		}
	}

	end = beg + 1;
	while(1)
	{
		ch = str.at(end);
		if((ch < 'A' || ch > 'z') || ( 'Z' < ch && ch <'a'))
		{
			break;
		}
		else
		{
			if(++end < str.length())
			{
				continue;
			}
			else
			{
				break;
			}
		}
	}
	
	return true;
}

ostream& operator << (ostream& out, mapWords& m)
{
	mapWords::iterator iter_map;
	vector<int>::iterator iter_vc;
	for(iter_map = m.begin(); iter_map != m.end(); ++iter_map)
	{
		out << iter_map->first << ":" << iter_map->second.nTimes << ":";
		for(iter_vc = iter_map->second.vc.begin(); iter_vc != iter_map->second.vc.end(); ++iter_vc)
		{
			cout << *iter_vc << "," ;
		}

		out << "  ";
	}

	out << endl;

	return out;
}
