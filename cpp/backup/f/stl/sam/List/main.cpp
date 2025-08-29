#include<iostream>
#include<list>
#include<algorithm>
#include<string>

using namespace std;

#define MAX_NUM 10
#define MAX_BUFFER_SIZE 64
typedef struct _list_data
{
	int nPos;
	char strData[MAX_BUFFER_SIZE];

	bool operator == (const _list_data data)
	{
		return nPos == data.nPos;
	}
}ListData,*PListData;

class CMyListSort
{
public:
	CMyListSort() {}
	bool operator()(const ListData& m,const ListData& n)
	{
		return m.nPos > n.nPos;    //会产生升序排序,若改为>,则变为降序
	}
};

class CMyListFindif
{
public:
	CMyListFindif(const ListData& val):m_ListVal(val){}
	bool operator () (const list<ListData>::value_type& other)
	{
		return  other.nPos  < m_ListVal.nPos;
	}

private:
	ListData m_ListVal;

};


ostream& operator << (ostream& out,  list<ListData>& lst);
void InitData(list<ListData>& lst);


int main(int argc, char* argv[])
{
	list<ListData> lst;
	InitData(lst);
	cout << lst;

	//sort(lst.begin(), lst.end(), CmpMyList);	//标准库的用不了
	lst.sort(CMyListSort());
	cout << lst;
	
	ListData val;
	val.nPos = 3;
	string strBase = "tie";
	memcpy(val.strData, strBase.data(), strBase.length()+ 1);
	list<ListData>::iterator it = find_if(lst.begin(),lst.end(), CMyListFindif(val));
	if(it == lst.end())
	{
		cout << "find data failed!\n" << endl;
	}
	else
	{
		it = lst.erase(it);
	}

	it = unique(lst.begin(),lst.end());		//需要重载==
	cout << lst;
	lst.erase(it, lst.end());
	cout << lst;
	lst.reverse();
	cout << lst;

	val.nPos = 7;
	it = find(lst.begin(), lst.end(), val);	//需要重载==

	return 0;
}

void InitData(list<ListData>& lst)
{
	ListData data;
	string strBase;
	for(int i = 0; i < MAX_NUM; ++i)
	{
		data.nPos = rand() % MAX_NUM;
		//sprintf(const_cast<char*>(strBase.c_str()),"s%d", data.nPos);  //good
		sprintf(data.strData,"s%d", data.nPos);
		lst.push_front(data);
	}

	return ;
}

ostream& operator << (ostream& out,  list<ListData>& lst)
{
	list<ListData>::iterator iter;
	for(iter = lst.begin(); iter != lst.end(); ++iter)
	{
		out << iter->nPos << ":" << iter->strData << "  ";
	}
	out << endl;

	return out;
}

