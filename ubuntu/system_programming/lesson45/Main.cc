#include "Thread.hpp"

void Loop()
{
    char name[64];
    pthread_getname_np(pthread_self(), name, sizeof(name));
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "new thread running" << std::endl;
        sleep(1);
    }
}



int main()
{
    ThreadModule::Thread t(Loop);
    t.Start();

    sleep(1);

    t.Detach();

    sleep(5);

    t.Stop();

    sleep(1);

    t.Join();

    t.PrintInfo();
    return 0;
}