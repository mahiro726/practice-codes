#include <iostream>
#include "Http.hpp"
#include <memory.h>
#include <cstring>
#include "TcpServer.hpp"

void Login(HttpRequest& req, HttpResponse& resp)
{
    std::cout << "Login function is called!" << std::endl;
    req.DebugPrint();

    // 再获取请求的正文数据，并打出来
    std::string data = req.ReqContent();
    std::cout << "data is " << data << std::endl;

    // 设置应答
    resp.SetCode(200);
    resp.SetBody("Login success!");
    resp.AddHeader("Content-Type", MiniType::Suffix2Mimemap(".txt"));
    resp.AddHeader("Content-Length", std::to_string(resp.BodySize()));
}

void Connect(HttpRequest& req, HttpResponse& resp)
{
    
}

void Register(HttpRequest& req, HttpResponse& resp)
{
}

void Search(HttpRequest& req, HttpResponse& resp)
{
}


static void Usage(const std::string &proc)
{
    std::cout << "Usage:\n\t" << proc << " server_ip server_port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    ENABLE_CONSOLE_LOG_STRATEGY();
    uint16_t server_port = std::stoi(argv[1]);


    // 定义协议对象
    std::unique_ptr<HttpProtocol> httpprotocol = std::make_unique<HttpProtocol>();
    // 注册服务
    httpprotocol->RegisterService("/Login", Login);
    httpprotocol->RegisterService("/Register", Register);
    httpprotocol->RegisterService("/Search", Search);

    // 定义网络对象
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(
        [&httpprotocol](std::string& inbuffer)->std::string
        {
            return httpprotocol->HandlerHttpRequest(inbuffer);
        }, server_port
    );

    // 启动
    tsvr->Loop();

    return 0;
}