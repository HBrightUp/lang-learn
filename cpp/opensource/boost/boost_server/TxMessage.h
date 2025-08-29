/*
 * @CopyRight:
 * TIANHE-CHAIN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TIANHE-CHAIN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TIANHE-CHAIN.  If not, see <http://www.gnu.org/licenses/>
 * (c) 2016-2018 tianhe-dev contributors.
 */
/**
 * @file: Message.h
 * @author: monan
 *
 * @date: 2017
 */

#pragma once

#include <libdevcore/Common.h>
#include <libdevcore/FixedHash.h>
#include <libethcore/Protocol.h>
#include <boost/lexical_cast.hpp>
#include <memory>
#include <queue>
#include <string>
#include <thread>
#include <arpa/inet.h>

using namespace dev ;

namespace Tx_Join
{


enum ProtocolID
{
    TX_TRAN_NO = 1,
    TX_BLOCK_NO = 2,
    TX_GET_BLOCK_NO =3,
    TX_PATCH_TRAN_NO = 4,  // this is nomal code decode patch , no simple code decode
    TX_RPC_PATCH_TRAN_NO = 5,  // this is nomal code decode patch , no simple code decode
    TX_SIM_TRAN_NO = 6 // simple code decode 
};

typedef  struct block_pend_t
{
    int64_t block_no;
    int64_t pend_size;
}BLOCK_AND_PEND;

class TxMessage : public std::enable_shared_from_this<TxMessage>
{
public:
    typedef std::shared_ptr<TxMessage> Ptr;

    TxMessage() { m_data = std::make_shared<bytes>(); }
    virtual ~TxMessage(){};

    virtual void encode(bytes& buffer) = 0;

    virtual ssize_t decode(const byte* buffer, size_t size) = 0;

    virtual uint32_t length() { return m_length; }

    virtual uint16_t type() { return m_type; }
    virtual void setType(uint16_t type) { m_type = type; }

    virtual std::string seq() { return m_seq; }
    virtual void setSeq(std::string seq) { m_seq = seq; }

    virtual int result() { return m_result; }
    virtual void setResult(int result) { m_result = result; }

    virtual byte* data() { return m_data->data(); }
    virtual size_t dataSize() { return m_data->size(); }

    virtual void setData(const byte* p, size_t size) { m_data->assign(p, p + size); }

    virtual void clearData() { m_data->clear(); }

    // for network statistic
    virtual void setGroupID(GROUP_ID const& _groupId) { m_groupId = _groupId; }
    GROUP_ID const& groupID() const { return m_groupId; }

protected:
    uint32_t m_length = 0;
    uint16_t m_type = 0;
    std::string m_seq = std::string(32, '0');
    int m_result = 0;
    dev::GROUP_ID m_groupId = 1;

    std::shared_ptr<bytes> m_data;
};

class TxMessageFactory : public std::enable_shared_from_this<TxMessageFactory>
{
public:
    typedef std::shared_ptr<TxMessageFactory> Ptr;

    virtual ~TxMessageFactory(){};
    virtual TxMessage::Ptr buildMessage() = 0;
};


class PatchTxMessage : public TxMessage
{
public:
    typedef std::shared_ptr<PatchTxMessage> Ptr;

    PatchTxMessage() : TxMessage() {}
    virtual ~PatchTxMessage() {}

    virtual void encode(bytes& buffer)
    {
        m_length = HEADER_LENGTH + m_data->size();
        uint32_t lengthN = htonl(HEADER_LENGTH + m_data->size());
        uint16_t typeN = htons(m_type);
        int32_t resultN = htonl(m_result);

        buffer.insert(buffer.end(), (byte*)&lengthN, (byte*)&lengthN + sizeof(lengthN));
        buffer.insert(buffer.end(), (byte*)&typeN, (byte*)&typeN + sizeof(typeN));
        buffer.insert(buffer.end(), m_seq.data(), m_seq.data() + m_seq.size());
        buffer.insert(buffer.end(), (byte*)&resultN, (byte*)&resultN + sizeof(resultN));

        buffer.insert(buffer.end(), m_data->begin(), m_data->end());
    }

    virtual ssize_t decode(const byte* buffer, size_t size) { return decodeAMOP(buffer, size); }

    virtual ssize_t decodeAMOP(const byte* buffer, size_t size)
    {
        if (size < HEADER_LENGTH)
        {
            return 0;
        }

        m_length = ntohl(*((uint32_t*)&buffer[0]));

#if 0
        if (_length > MAX_LENGTH)
        {
            return -1;
        }
#endif

        if (size < m_length)
        {
            return 0;
        }

        m_type = ntohs(*((uint16_t*)&buffer[4]));
        m_seq.assign(&buffer[6], &buffer[6] + 32);
        m_result = ntohl(*((uint32_t*)&buffer[38]));

        m_data->assign(&buffer[HEADER_LENGTH], &buffer[HEADER_LENGTH] + m_length - HEADER_LENGTH);

        return m_length;
    }

protected:
    const static size_t MIN_HEADER_LENGTH = 4;

    const static size_t HEADER_LENGTH = 4 + 2 + 32 + 4;
    const static size_t MAX_LENGTH = ULONG_MAX;  // max 4G
};

class  PatchMessageFactory : public TxMessageFactory
{
public:
    virtual ~PatchMessageFactory(){};
    virtual TxMessage::Ptr buildMessage() override { return std::make_shared<PatchTxMessage>(); }
};


}  // namespace Tx_Join


