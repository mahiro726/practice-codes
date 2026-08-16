#include "Thread.hpp"
#include "Mutex.hpp"

int tickets = 1000;
Mutex lock;

void GetTicket()
{
    char name[64];
    pthread_getname_np(pthread_self(), name, sizeof(name));

    while (1)
    {
        printf("--------------------临界区--------------------\n");
        {
            LockGuard lockGuard(lock);
            if (tickets > 0)
            {
                usleep(10000);
                printf("%s 抢到了ticket: %d\n", name, tickets);
                tickets--;
            }
            else
            {
                break;
            }
        }
        printf("---------------------------------------------\n");
    }
}

int main()
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    ThreadModule::Thread t1(GetTicket);
    ThreadModule::Thread t2(GetTicket);
    ThreadModule::Thread t3(GetTicket);

    t1.Start();
    t2.Start();
    t3.Start();

    t1.Join();
    t2.Join();
    t3.Join();

    pthread_mutex_destroy(&mutex);
    return 0;
}