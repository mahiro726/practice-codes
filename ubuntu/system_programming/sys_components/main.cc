#include <iostream>
#include <unistd.h>
#include "BlockQueue.hpp"

BlockQueue<int> bq(5);

void* Productor(void* args)
{
    static int i = 1;
    while(true)
    {
        bq.Enqueue(i++);
        std::cout << "生产：" << i << std::endl;
        sleep(1);
    }
    return nullptr;
}

void* Consumer(void* args)
{
    while(true)
    {
        int out;
        bq.pop(&out);
        std::cout << "消费：" << out << std::endl;
        sleep(1);
    }
    return nullptr;
}

int main()
{
    pthread_t consumer_thread;
    pthread_t productor_thread;
    pthread_create(&consumer_thread, nullptr, Productor, nullptr);
    pthread_create(&productor_thread, nullptr, Consumer, nullptr);

    pthread_join(consumer_thread, nullptr);
    pthread_join(productor_thread, nullptr);

    return 0;
}