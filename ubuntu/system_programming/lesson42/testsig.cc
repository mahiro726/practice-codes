#include <iostream>
#include <signal.h>

void handler(int signo)
{
    std::cout << "捕捉一个信号：" << signo << std::endl;

    sigset_t pending;
    while (true)
    {
        sigpending(&pending);
        for (int i = 31; i > 0; i--)
        {
            if (sigismember(&pending, i))
            {
                std::cout << "1";
            }
            else
                std::cout << "0";
        }
        std::cout << std::endl;
        sleep(1);
    }
}

int main()
{
    struct sigaction act, oact;

    act.sa_handler = handler;
    sigemptyset(&(act.sa_mask));
    
    sigaddset(&(act.sa_mask), 3); // 比signal多一个功能，在捕获2号信号的同时，如果想屏蔽其他信号，可以像这样加进去
    sigaddset(&(act.sa_mask), 4);
    sigaddset(&(act.sa_mask), 5);


    // 初始化，防止flag出现乱码
    act.sa_flags = 0;
    act.sa_restorer = nullptr;

    sigaction(SIGINT, &act, &oact);

    while (true)
    {
        std::cout << "进程正在运行, pid: " << getpid() << std::endl;
        sleep(1);
    }
}