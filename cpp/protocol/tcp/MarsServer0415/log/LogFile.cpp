#include "LogFile.h"


CLogFile::CLogFile()
{
    m_strLogPath = "./logfile/";
}

CLogFile::~CLogFile()
{

}

CLogFile* CLogFile::GetInstance()
{
    static CLogFile* pLog = 0;
    if(pLog == 0)
    {
        pLog = new CLogFile();
        pLog->InitDirectory();
    }

    return pLog;
}

void CLogFile::InitDirectory()
{




}
