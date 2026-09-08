#include "ThreadPool.hpp" // 人力资源
#include "UdpServer.hpp"  // 获取事件
#include "Route.hpp"      // 任务，怎么处理事件
#include <memory> // 智能指针
#include <functional>

using task_t = std::function<void()>;

static void Usage(const std::string &process)
{
    std::cerr << "Usage:\n\t";
    std::cerr << process << " local_port" << std::endl;
}
    
// ./server_udp port
int main(int argc, char* argv[])
{
    ENABLE_CONSOLE_LOG_STRATEGY();

    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERROR);
    }
    std::uint16_t server_port = std::stoi(argv[1]);

    // 线程池模块
    auto threadpool = ThreadPool<task_t>::GetInstance();  // 单例模式下，是创建即启动的，所以不需要手动Start 

    // 路由模块
    Route r;    

    // 网络模块
    UdpServer usvr(server_port);

    usvr.Init();
    usvr.RegisterService([&r](const InetAddr& inetaddress){
        r.CheckUser(inetaddress);  // 为什么登记用户不加入花名册呢，因为这是微妙操作，交给线程池反而会降效，还有则是登记十分重要必须在广播前面，所有需要同步操作
    },                             // 异步操作有乱序风险
    [&r, threadpool](int sockfd, std::string msg){  // lambda相当于将工具都包装好了，你可以理解为做成了一个类，所以调用的文件甚至不需要见过threadpool也能用
        auto t = std::bind(&Route::Broadcast, &r, sockfd, msg); // 跟lambda表达式的效果一致
        threadpool->Enqueue(t);
    });

    usvr.Start();

    return 0;
}