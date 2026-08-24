#ifndef __ECHOSERVER_HPP
#define __ECHOSERVER_HPP

#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Logger.hpp"


using namespace NS_LOG_MODULE;

const static int default_fd = -1;
const static int default_port = 8888;

enum
{
    SUCCESS = 0,
    USAGE_ERR,
    SOCKET_ERR,
    BIND_ERR
};

class UdpServer
{
public:
    //UdpServer(const std::string &ip, uint16_t port = default_port)
    UdpServer(uint16_t port = default_port)
    :_sockfd(default_fd),
     _port(port)
    {}

    void Init()
    {
        // 第一步：创建socket, 本质：打开网卡
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            LOG(LogLevel::FATAL) << "create socket error";
            exit(SOCKET_ERR);
        }
        LOG(LogLevel::INFO) << "create socket success, sockfd: " << _sockfd;

        // 第二步：填充网络信息，有没有IP和端口号设置到内核中？？
        struct sockaddr_in local; // struct sockaddr_in 数据类型，local 用户栈上的！！！，并没有设置到内核
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        // local.sin_addr.s_addr = inet_addr(_ip.c_str()); // 1. 字符串ip->4字节IP  2. hton
        local.sin_addr.s_addr = INADDR_ANY; // 最佳实践：任意IP地址绑定
        local.sin_port = htons(_port);

        // 第三步：bind socket 信息
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "bind socket error";
            exit(BIND_ERR); 
        }
        LOG(LogLevel::INFO) << "bind socket success" << ", port: " << _port;
    }

    void Start()
    {
        char inbuffer[1024];
        while(true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            // 1. 用户发来的数据
            // 2. 用户的socket信息
            ssize_t n = recvfrom(_sockfd, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (n > 0)
            {
                // 用户的IP + Port信息，是recv from从网络中获取数据，网络序列（大端）-> 转换成主机序列
                // peer.sin_port; // peer client port
                // peer.sin_addr; // peer client ip
                uint16_t client_port = ntohs(peer.sin_port);
                std::string client_ip = inet_ntoa(peer.sin_addr);  // 4字节IP -> ntoh -> 字符串
                std::string client_address = "[" + client_ip + ":" + std::to_string(client_port) + "]";

                // 用户发来的数据
                inbuffer[n] = 0;
                LOG(LogLevel::DEBUG) << client_address << inbuffer;
                std::string echo_string = "server echo# ";
                echo_string += inbuffer;
                // h to n
                sendto(_sockfd, echo_string.c_str(), echo_string.size(), 0, (struct sockaddr *)&peer, len);
            }
            else 
            {
                LOG(LogLevel::ERROR) << "recvfrom error";
            }

        }
    }

    ~UdpServer()
    {
        close(_sockfd);
    }

private:
    int _sockfd;
    // std::string _ip; // "192.168.2.2" && 4字节IP
    uint16_t _port;
};

#endif