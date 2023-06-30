#include <pthread.h>

pthread_mutex_t* getMutex();
int foo();

void my_lock(pthread_mutex_t* mutex)
{
    pthread_mutex_lock(mutex);
}

void my_unlock(pthread_mutex_t* mutex)
{
    pthread_mutex_unlock(mutex);
}

void test_custom_lock_fuctions()
{
    pthread_mutex_t* mutex = getMutex();
    my_lock(mutex);
    if (foo()) {
        my_unlock(mutex);
        return;
    }
    my_unlock(mutex);
}
