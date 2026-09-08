#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "UserManager.hpp"

class Route  // 相当于只是在UserManager上加了一层锁
{
public:
    Route()
    :_uma(std::make_unique<UserManager>())
    {}

    void Broadcast(int sockfd, std::string msg)
    {
        // 广播的时候要加锁
        LockGuard lockguard(_mutex);
        for (auto& user : _uma->GetUsers())
        {
            sendto(sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr*)user.GetAddress(), user.GetLen());
        }
    }

    void CheckUser(const InetAddr& inetaddress)
    {
        LockGuard lockguard(_mutex);
        _uma->AddUser(inetaddress);
    }

    void OfflineUser(const InetAddr& inetaddress)
    {
        LockGuard lockguard(_mutex);
        _uma->DeleteUser(inetaddress);
    }

    ~Route()
    {}

private:
    std::unique_ptr<UserManager> _uma;
    Mutex _mutex;
};