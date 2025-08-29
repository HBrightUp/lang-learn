#ifndef CSERVERCONFIG
#define CSERVERCONFIG
#include<iostream>
#include<cstring>
#include<map>
#include<vector>

using namespace std;

#define MAX_FIELD 2

class CServerConfig
{
public:
    void SetFilePath(string str);
    void Init();
    bool Parse();

private:
    bool IsConfigKey(const string& str);

private:
    string m_strFilePath;
    map<string, string> m_mapConfig;

};







#endif // CSERVERCONFIG

