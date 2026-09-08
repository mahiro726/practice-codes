#include <iostream>
#include <string>

#include <strings.h>
#include "Thread.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "InetAddr.hpp"

int sockfd = 0;
std::string server_ip = "0.0.0.0";
uint16_t server_port = 0;
std::string nickname;

void Usage(const std::string &process)
{
    std::cerr << "Usage:\n\t";
    std::cerr << process << " server_ip server_port";
}

void Online(const InetAddr& serveraddress)
{
    std::cout << "Please Enter Your Nickname# ";  // 1
    std::getline(std::cin, nickname);
    std::string online_message = nickname + " online!";
    ssize_t n = sendto(sockfd, online_message.c_str(), online_message.size(), 0, (struct sockaddr*)serveraddress.GetAddress(), serveraddress.GetLen());
    (void)n;
}

void RecvMessage()
{
    char inbuffer[1024];
    while(true)
    {
        struct sockaddr_in tmp;
        socklen_t len = sizeof(tmp);
        ssize_t n = recvfrom(sockfd, inbuffer, sizeof(inbuffer)-1, 0, (struct sockaddr*)&tmp, &len);
        if (n >= 0)
        {
            inbuffer[n] = 0;
            std::cerr << inbuffer << std::endl; // 收到的消息，2
        }
        else 
        {
            std::cerr << "客户端 recvfrom error" << std::endl;
            continue;
        }
    }
}

void SendMessage()
{    
    InetAddr serveraddress(server_port, server_ip);
    Online(serveraddress);

    while(true)
    {
        // 获取用户输入
        std::string message;
        std::cout << "Please Enter# "; // 获取用户输入的请求，1
        std::getline(std::cin, message);

        message = nickname + "# " + message;

        // 发送给server，首次发送自动bind
        ssize_t n = sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)serveraddress.GetAddress(), serveraddress.GetLen());
        (void)n;
        
    }
}

int main(int argc, char *args[])
{
    if (argc != 3) // 输入的合法性检验
    {
        Usage(args[0]);
        exit(1);
    }

    server_ip = args[1];      // 从输入中接收服务端ip
    server_port = atoi(args[2]); // 接收port

    // 建立通道
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "客户端 socket 创建通道失败" << std::endl;
        exit(2);
    }

    Thread receiver(RecvMessage);
    Thread sender(SendMessage);

    receiver.Start();
    sender.Start();

    receiver.Join();
    sender.Join();

    return 0;
}