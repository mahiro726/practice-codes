#pragma once
#include <pthread.h>
#include <queue>

const int defaultcap = 5;

template <class T>
class BlockQueue
{
public:
    BlockQueue(int cap = defaultcap)
    :_cap(cap)
    {
        pthread_mutex_init(&_lock, nullptr);
        pthread_cond_init(&_productor_cond, nullptr);
        pthread_cond_init(&_consumer_cond, nullptr);
    }

    void Enqueue(const T& in)
    {
        pthread_mutex_lock(&_lock);

        // 判满
        while (_bq.size() == _cap)
        {
            // 满则等
            pthread_cond_wait(&_productor_cond, &_lock);
        }
        // 入数据
        _bq.push(in);
        
        pthread_cond_signal(&_consumer_cond);

        pthread_mutex_unlock(&_lock);
    }

    void pop(T* out) // 输出型参数
    {
        pthread_mutex_lock(&_lock);

        // 判空
        while (_bq.empty())
        {
            // 空则等
            pthread_cond_wait(&_consumer_cond, &_lock);
        }
        // 取数据
        *out = _bq.front();
        _bq.pop();

        pthread_cond_signal(&_productor_cond);

        pthread_mutex_unlock(&_lock);
    }


    ~BlockQueue()
    {
        pthread_mutex_destroy(&_lock);
        pthread_cond_destroy(&_productor_cond);
        pthread_cond_destroy(&_consumer_cond);
    }

private:
    // 需求1：交易场所
    // 需求2：判满
    // 需求3：保护临界资源
    // 需求4：生产者条件
    // 需求5：消费者条件
    std::queue<T> _bq; 
    int _cap;
    pthread_mutex_t _lock;
    pthread_cond_t _productor_cond;
    pthread_cond_t _consumer_cond;
};