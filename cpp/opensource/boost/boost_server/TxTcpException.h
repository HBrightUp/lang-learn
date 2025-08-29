

#pragma once

#include <string>

#define TXJOIN_LOG(LEVEL) LOG(LEVEL) << "[TXJOIN]"

#define TXJOIN_SESSION_LOG(LEVEL) LOG(LEVEL) << "[TXJOIN][SESSION]"

#pragma warning(push)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include <boost/exception/diagnostic_information.hpp>
#pragma warning(pop)
#pragma GCC diagnostic pop

namespace Tx_Join 
{

class TxJoinException : public std::exception
{
public:
    TxJoinException(){};
    TxJoinException(int _errorCode, const std::string& _msg)
      : m_errorCode(_errorCode), m_msg(_msg){};

    virtual int errorCode() { return m_errorCode; };
    virtual const char* what() const noexcept override { return m_msg.c_str(); };

    bool operator!() const { return m_errorCode == 0; }

private:
    int m_errorCode = 0;
    std::string m_msg = "";
};

} // Tx_Join