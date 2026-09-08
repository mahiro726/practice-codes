#pragma once

#include <iostream>
#include <functional>
#include <unistd.h>
#include "InetAddr.hpp"
#include "Logger.hpp"
#include <cstdint> // uint16_t

#include <strings.h> // bzero

#include <sys/socket.h> // socket、bind
#include <netinet/in.h> // struct sockaddr_in 结构体
#include <arpa/inet.h>  // htons、inet_addr

using namespace NS_LOG_MODULE;
const uint16_t defaultport = 8888;

// 我作为udpserver，我只读数据，关于之后怎么处理数据，我不管，都给上层就行了
using handler_addr_t = std::function<void(const InetAddr&)>; // 我只是拿到地址，然后处理地址，但是处理地址的细节我不关心，直接交给上层去处理
using handler_msg_t = std::function<void(int , std::string)>;

enum
{
    SUCCESS,
    SOCKET_ERROR,
    BIND_ERROR,
    USAGE_ERROR
};

class UdpServer
{
public:
    UdpServer(uint16_t port = defaultport)
        : _socketfd(-1),
          _port(port)
    {}

    void Init() // 初始化依旧先建立网络通道，通过socket函数打开网络文件
    {
        // 1. 创建socket
        // 2. 填网络属性
        // 3. bind，将 IP 和 端口 写进内核
        _socketfd = socket(AF_INET, SOCK_DGRAM, 0);

        if (_socketfd < 0)
        {
            LOG(LogLevel::FATAL) << "create socket error";
            exit(SOCKET_ERROR);
        }

        LOG(LogLevel::INFO) << "create socket success";

        // struct sockaddr_in local;
        // bzero(&local, sizeof(local));                   // 清零，防脏值
        // local.sin_family = AF_INET;                     // 协议家族
        // // local.sin_addr.s_addr = inet_addr(_ip.c_str()); // sin_addr是一个结构体，要用它的成员变量s_addr来接收
        // local.sin_port = htons(_port);                  // 端口：主机序->网络序
        // local.sin_addr.s_addr = INADDR_ANY;             // 最佳实践，采用任意ip绑定

        InetAddr local(_port);

        int ret = bind(_socketfd, (struct sockaddr *)local.GetAddress(), local.GetLen());
        if (ret < 0)
        {
            LOG(LogLevel::FATAL) << "bind error";
            exit(BIND_ERROR);
        }
        LOG(LogLevel::INFO) << "bind success";
    }

    void RegisterService(handler_addr_t handler_addr, handler_msg_t handler_msg) // 如果写到构造函数中就太冗余了，所以独立成一个函数
    {
        _handler_addr = handler_addr;
        _handler_msg = handler_msg;
    }

    void Start()
    {
        char inbuffer[1024];

        while (true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            ssize_t n = recvfrom(_socketfd, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr*)&peer, &len); // 一般读取类的都是返回实际读到的字节数

            if (n >= 0) // 拿到信息后的处理
            {
                inbuffer[n] = 0;
                // 检测新用户
                InetAddr clientaddress(peer);
                _handler_addr(clientaddress);
                
                std::string ip_port = clientaddress.ToString();
                std::string message = ip_port + inbuffer;
                LOG(LogLevel::DEBUG) << message;

                // 处理收到的数据，即转发消息
                // nickname# message -> [ip:port]-nickname# message

                _handler_msg(_socketfd, message);
            }
            else
            {
                LOG(LogLevel::ERROR) << "recvfrom error";
            }
        }
    }

    ~UdpServer()
    {
        close(_socketfd);
    }
private:
    int _socketfd;
    uint16_t _port;
    handler_addr_t _handler_addr;
    handler_msg_t _handler_msg;
};