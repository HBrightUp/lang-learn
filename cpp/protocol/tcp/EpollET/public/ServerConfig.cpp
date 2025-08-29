#include"ServerConfig.h"
#include<fstream>


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
#if 0
    map<string,string>::iterator iter;
    for(iter = m_mapConfig.begin(); iter != m_mapConfig.end(); ++iter)
    {
        cout << iter->first << "  "  << iter->second << endl;
    }

#endif
    file.close();
}

bool CServerConfig::IsConfigKey(const string &str)
{
    const string strStd[] = {"HOST", "PORT","USER_DB_HOST","USER_DB_PORT", "USER_DB_NAME", "USER_DB_USERNAME" ,"USER_DB_PASSWORD"};

    for(int i = 0; i < 7; ++i)
    {
        if(strStd[i] == str)
        {
            return true;
        }
    }

    return false;
}






