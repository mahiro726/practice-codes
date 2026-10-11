#include "EchoServer.hpp"
#include <string>
#include <memory>
#include "Dict.hpp"

static void Usage(const std::string &process)
{
    std::cerr << "Usage:\n\t";
    std::cerr << process << " local_ip local_port" << std::endl;
}

// ./server_udp port
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }

    ENABLE_CONSOLE_LOG_STRATEGY();
    // 1. 定义字典
    Dict dict;
    // 2. 构建网络服务
    uint16_t server_port = std::stoi(argv[1]);
    // 3. 绑定上下两层
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>([&dict](std::string word)->std::string{
        return dict.Translate(word);
    }, 
    server_port);
    
    usvr->Init();

    usvr->Start();

    return 0;
}