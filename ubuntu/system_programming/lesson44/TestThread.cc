#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <unistd.h>
#include "Task.hpp"
#include <ctime>
#include <sys/syscall.h>

using std::cout;
using std::endl;

pid_t GetTid()
{
    syscall(SYS_gettid);
}

// 线程局部存储: 只能用来局部存储内置类型，常见的是整形。
__thread pid_t id = 0;

void* routine(void* args)
{
    std::string name = static_cast<const char*>(args);
    while(true)
    {
        cout << "new thread id: " << id << endl;
        printf("new thread id: %p\n", &id);

        id++;
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, routine, (void*)"thread-1");

    while(true)
    {
        cout << "main thread id: " << id << endl;
        printf("main thread id: %p\n", &id);
        sleep(1);
    }

    pthread_join(tid, nullptr);
    return 0;
}
