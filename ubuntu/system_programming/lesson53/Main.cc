#include "BlockQueue.hpp"

int num = 1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int Getnumber()
{
    pthread_mutex_lock(&lock);
    int number = num++;
    pthread_mutex_unlock(&lock);

    return number;
}

void* ConsumerRoutine(void* args)
{
    int number = Getnumber();
    std::string name = "ConsumerThread-" + std::to_string(number);
    pthread_setname_np(pthread_self(), name.c_str());


    BlockQueue<int>* bq = static_cast<BlockQueue<int>*>(args);
    while(1)
    {
        int data;
        bq->Pop(&data);
        std::cout << "消费：" << data << std::endl;
    }
}

void* ProductorRoutine(void* args)
{
    int number = Getnumber();
    std::string name = "ProductorThread-" + std::to_string(number);
    pthread_setname_np(pthread_self(), name.c_str());
    

    BlockQueue<int>* bq = static_cast<BlockQueue<int>*>(args);
    int data = 10;
    while(1)
    {        
        bq->Enqueue(data);
        std::cout << "生产：" << data++ << std::endl;
    }
}

int main()
{
    BlockQueue<int> *bq = new BlockQueue<int>(); // 传给线程的参数的生命周期必须大于此线程的生命周期
    pthread_t c, p;
    pthread_create(&c, nullptr, ConsumerRoutine, (void*) bq);
    pthread_create(&p, nullptr, ProductorRoutine, (void*) bq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    return 0;
}