#ifndef _LOGIC_NET_SESSION_H
#define _LOGIC_NET_SESSION_H

#include <stdint.h>
#include "WrapTCPService.h"

#define USE_DELAY_SEND

class BaseLogicSession
{
public:
    typedef std::shared_ptr<BaseLogicSession> PTR;

    BaseLogicSession()
    {
    }

    virtual ~BaseLogicSession()
    {}

    void            setSession(WrapServer::PTR server, int64_t socketID, const std::string& ip)
    {
        mServer = server;
        mSocketID = socketID;
        mIP = ip;
    }

    virtual void    onEnter() = 0;
    virtual void    onClose() = 0;
    virtual void    onMsg(const char* buffer, int len) = 0;

    void            send(const char* buffer, int len)
    {
        mServer->getService()->send(mSocketID, DataSocket::makePacket(buffer, len));
    }

    void            send(const DataSocket::PACKET_PTR& packet)
    {
        mServer->getService()->send(mSocketID, packet);
    }

    int64_t         getSocketID() const
    {
        return mSocketID;
    }

    void            cacheSend(const char* buffer, int len)
    {
#ifdef USE_DELAY_SEND
        mServer->getService()->cacheSend(mSocketID, DataSocket::makePacket(buffer, len));
#else
        mServer->getService()->send(mSocketID, DataSocket::makePacket(buffer, len));
#endif
    }
    void            cacheSend(const DataSocket::PACKET_PTR& packet)
    {
#ifdef USE_DELAY_SEND
        mServer->getService()->cacheSend(mSocketID, packet);
#else
        mServer->getService()->send(mSocketID, packet);
#endif
    }
private:
    WrapServer::PTR     mServer;
    int64_t             mSocketID;
    std::string         mIP;
};

#endif