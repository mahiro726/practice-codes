#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <signal.h>

void PrintPending(sigset_t &pending)
{
    for (int signo = 31; signo > 0; signo--)
    {
        if (sigismember(&pending, signo))
        {
            std::cout << "1";
        }
        else
        {
            std::cout << "0";
        }
    }
    std::cout << std::endl;
}

void myhandler (int signo)
{
    std::cout << "-----------------------entry myhandler" << std::endl;
    sigset_t pending;
    sigemptyset(&pending);
    sigpending(&pending);
    PrintPending(pending);  // 验证在执行handler之前就将pending中2信号的位置置为0了
    std::cout << "处理完成： " << signo << std::endl; // 更改2号的默认终止行为，改为打印
    std::cout << "-----------------------leave myhandler" << std::endl;
}

int main()
{
    // 0. 捕捉2号信号
    signal(2, myhandler);

    // 1，屏蔽2号信号
    sigset_t block_set, old_set;
    sigemptyset(&block_set);
    sigemptyset(&old_set);

    sigaddset(&block_set, SIGINT); // 截止到现在，有没有把2号信号屏蔽？ -- 没有!

    int n = sigprocmask(SIG_SETMASK, &block_set, &old_set);
    (void)n;

    std::cout << "我的pid: " << getpid() << std::endl;

    int cnt = 1;
    while(true)
    {
        sigset_t pending;
        sigemptyset(&pending);
        // 2. 获取pending表
        sigpending(&pending);
        // 3. 打印pending表
        PrintPending(pending);

        if (cnt == 20)
        {
            // 4. 解除对2号信号的屏蔽
            std::cout << "解除对2号信号屏蔽" << std::endl;
            int n = sigprocmask(SIG_SETMASK, &old_set, nullptr);
            (void)n;
        }
        cnt++;
        sleep(1);
    }
}
