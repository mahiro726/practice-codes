#ifndef __BLOCK_QUEUE_H
#define __BLOCK_QUEUE_H

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <queue>

const int defaultcap = 5;

template <typename T>
class BlockQueue
{
public:
    BlockQueue(int cap = defaultcap)
    :_cap(cap)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_consumer_cond, nullptr);
        pthread_cond_init(&_productor_cond, nullptr);

        sleep_productor_num = 0;
        sleep_consumer_num = 0;
    }

    void Enqueue(T& in)
    {
        pthread_mutex_lock(&_mutex);
        
        while (_bq.size() == _cap)
        {
            sleep_productor_num++;
            pthread_cond_wait(&_productor_cond, &_mutex);
            sleep_productor_num--;

        }

        _bq.push(in);

        if (sleep_consumer_num > 0)
            pthread_cond_signal(&_consumer_cond); // 生产一次就将消费者唤醒
        pthread_mutex_unlock(&_mutex);
    }

    void Pop(T* out)
    {
        pthread_mutex_lock(&_mutex);
        while (_bq.empty())  // 为了增强代码的健壮性和鲁棒性，我们需要防御性编程，也就是将if改成while
        {
            sleep_consumer_num++;
            // 0. 为什么要判断啊？？
            // 1. 线程为什么会在临界区内部等待？？好奇怪？
            // 判断资源是否就绪，本质也是访问临界资源
            pthread_cond_wait(&_consumer_cond, &_mutex);
            sleep_consumer_num--;
        } 
        
        *out = _bq.front();
        _bq.pop();

        if (sleep_productor_num > 0)
            pthread_cond_signal(&_productor_cond); // 将生产者唤醒
        pthread_mutex_unlock(&_mutex);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_consumer_cond);
        pthread_cond_destroy(&_productor_cond);
    }

private:
    std::queue<T> _bq;
    int _cap;

    pthread_mutex_t _mutex;
    pthread_cond_t _consumer_cond;
    pthread_cond_t _productor_cond; 

    // 水位线
    // int _blockqueue_low_water; // 低水位线
    // int _blockqueue_hight_water; // 高水位线
    // sleep thread num

    int sleep_productor_num;
    int sleep_consumer_num;
};

#endif