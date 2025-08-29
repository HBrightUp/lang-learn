#ifndef CLOGFILE
#define CLOGFILE
#include "../public/PublicFun.h"

class CLogFile
{
private:
    CLogFile();
    ~CLogFile();

public:
    static CLogFile* GetInstance();



private:
    string m_strLogPath;
    void InitDirectory();

};






























#endif // CLOGFILE

