#pragma once
#include <iostream>
#include <pthread.h>

class Mutex
{
public:
    Mutex()
    {}

    void Lock()
    {
        pthread_mutex_lock(&_lock);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&_lock);
    }

    ~Mutex()
    {}
private:
    pthread_mutex_t _lock = PTHREAD_MUTEX_INITIALIZER;
};

class LockGuard
{
public:
    LockGuard(Mutex &lock)
    :_lockref(lock)
    {
        _lockref.Lock();
    }
    ~LockGuard()
    {
        _lockref.Unlock();
    }
private:
    Mutex &_lockref;
};