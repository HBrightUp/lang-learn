#include"ServerConfig.h"
#include<fstream>



void CServerConfig::Init()
{
  //   m_strRoot[1] = "MarsServer";

}

void CServerConfig::SetFilePath(string str)
{
    m_strFilePath = str;
}

bool CServerConfig::Parse()
{
    fstream file(m_strFilePath);
    if(!file.is_open())
    {
        printf("open file failed! strFilePath:%s\n", m_strFilePath);
        return false;
    }

    string strLine,strKey,strValue;
    int nPos = 0;


    while(getline(file, strLine) )
    {
        nPos = strLine.find('=');
        if(nPos <= 0)
            continue;

        strKey =  strLine.substr(0, nPos);

        if(IsConfigKey(strKey))
        {
            pair<string, string>  data(strKey, strLine.substr(nPos + 1, strLine.length()));
            m_mapConfig.insert(data);
        }
    }
    map<string,string>::iterator iter;
    for(iter = m_mapConfig.begin(); iter != m_mapConfig.end(); ++iter)
    {
        cout << iter->first << "  "  << iter->second << endl;
    }

    file.close();
}

bool CServerConfig::IsConfigKey(const string &str)
{
    const string strStd[6] = {"HOST", "PORT","USER_DB_HOST", "USER_DB_NAME", "USER_DB_USERNAME" ,"USER_DB_PASSWORD"};

    for(int i = 0; i < 6; ++i)
    {
        if(strStd[i] == str)
        {
            return true;
        }
    }

    return false;
}






