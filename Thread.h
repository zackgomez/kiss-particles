#pragma once
#include <pthread.h>
#include <semaphore.h>

/** 
 * Encapsulates a thread and contains methods for starting and stopping one.
 */
class Thread
{
public:
    Thread() : running_(false)
    { /* Empty */ }

    ~Thread()
    {
        kill();
    }

    void run( void* f(void*), void * arg)
    {
        pthread_create(&t_, NULL, f, arg);
        running_ = true;
    }

    void join()
    {
        if (running_)
            pthread_join(t_, NULL);
        running_ = false;
    }

    void kill()
    {
        if (running_)
        {
            pthread_cancel(t_);
            pthread_join(t_, NULL);
        }
        running_ = false;
    }

private:
    Thread(const Thread&);
    const Thread& operator=(const Thread&);

    bool running_;
    pthread_t t_;
};

/** 
 * Encapsulates a mutex.
 */
class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&m_, NULL);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&m_);
    }

    void lock()
    {
        pthread_mutex_lock(&m_);
    }

    void unlock()
    {
        pthread_mutex_unlock(&m_);
    }

private:
    pthread_mutex_t m_;
};


class Semaphore
{
public:
    Semaphore(int value = 0)
    {
        sem_init(&sem_, 0, value);
    }

    ~Semaphore()
    {
        sem_destroy(&sem_);
    }

    void inc()
    {
        sem_post(&sem_);
    }

    void dec()
    {
        sem_wait(&sem_);
    }

    int value()
    {
        int ret;
        sem_getvalue(&sem_, &ret);
        return ret;
    }

private:
    sem_t sem_;
};

