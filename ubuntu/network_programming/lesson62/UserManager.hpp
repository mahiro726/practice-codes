#pragma once

#include <iostream>
#include "InetAddr.hpp"
#include "Logger.hpp"
#include <vector>

using namespace NS_LOG_MODULE;

class UserManager
{
public:
    UserManager()
    {}

    bool SearchUser(const InetAddr& inetaddress) const
    {
        for(const auto& user : _users)
        {
            if (user == inetaddress)
            {
                return true;
            }
        }
        return false;
    }
    
    void AddUser(const InetAddr& inetaddress)
    {
        if (!SearchUser(inetaddress))
        {
            _users.push_back(inetaddress);
        }
    }

    void DeleteUser(const InetAddr& inetaddress)
    {
        for (auto it = _users.begin(); it != _users.end(); ++it)
        {
            if ((*it) == inetaddress)
            {
                _users.erase(it);
                break; // 找到第一个相等的就删除
            }   
        }
    }

    const std::vector<InetAddr>& GetUsers() const
    {
        return _users;
    }

    ~UserManager()
    {}

private:
    std::vector<InetAddr> _users; // 将 vector 作为花名册，用于登记用户
};