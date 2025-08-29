#include<iostream>
#include<string>
#include<algorithm>

using namespace std;

void TestMember();
void TestAlgorithm();

int main()
{
	//TestMember();
	
	return 0;
}

void TestMember()
{
	string str("Good monring!");
	cout << "Mystring:" << str << endl;
	cout << "str[3]:" << str[3] << endl;
	cout << "str.at[2]:" << str.at(2) << endl;
	cout << "str.data():" << str.data() << endl;
	cout << "str.c_str():" << str.c_str() << endl;

	char buf[1024];
	memset(buf, 0, sizeof(buf));

	int nSize = str.copy(buf, 3, 1);
	if(nSize == 3)
	{
		cout << "str.copy(buf, 3, 1)=> buf:" << buf << endl;
	}

	cout << "str.capacity():" << str.capacity() << endl;
	cout << "str.max_size():" << str.max_size() << endl;
	cout << "str.size():" << str.size() << endl;
	cout << "str.length():" << str.length() << endl;
	cout << "str.empty():" << str.empty() << endl;

	str.resize(20,'1');
	cout << "str.resize(20,'1'),str:" << str << endl;

	string strTemp = str.assign("Good monring!");
	cout << "strTmep:" << strTemp << "  str:" << str << endl;
	
	str.clear();
	cout << "str.clear(), str:" << str << endl;
	cout << "str.assign(strTemp, 2, 5),str:" << str.assign(strTemp, 2, 5) << endl;

	str +=strTemp;
	cout << str << endl;

	str.append("Bright!");
	cout << "str:" << str << endl;
	
	
	str.append("Bright", 3);
	cout << "str:" << str << endl;

	cout << "strTemp:" << strTemp << endl;
	str.append(strTemp,3, 4);
	cout << "str" <<   str << endl;
	

	str.clear();
	str.append("Good,morning!", 4);
	cout << str << endl;

	str = "Good,Bright";
	str ="monring";

	cout << "str:" << str  << " strTemp:" << strTemp << endl;
	cout << (str > strTemp) << (str < strTemp) << (str == strTemp) << (str != strTemp) << (str >= strTemp) << (str <= strTemp) << endl;
	
	strTemp = str.substr(2, 5);
	cout << "strTemp:" << strTemp << endl;	

	str.swap(strTemp);
	cout << "str:" << str  << " strTemp:" << strTemp << endl;
	
	cout << "str.compare(1, 3, strTemp, 2, 4),result:" << str.compare(1, 3, strTemp, 2, 4) << endl;
	
	int nPos = strTemp.find('n', 2);
	cout << "strTemp:" << strTemp << "char i,pos = " << nPos << endl;
	
	strTemp.replace(2, 5, "myfilmcome");
	cout << "strTemp:" << strTemp << endl;

	str.insert(2, "Bright");
	cout << "str:" << str << endl;

	str.erase(2,str.length() - 2 - 1);
	cout << "str:" << str << endl;
}
void TestAlgorithm()
{
	string strTemp = "Sometimes, I want to BeiJing to look look!";

	string str1("yesterday");
	string str2 = "future";

	
}