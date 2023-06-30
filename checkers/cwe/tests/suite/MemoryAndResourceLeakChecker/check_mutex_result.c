#include <pthread.h>

pthread_mutex_t t;

int test1()
{
    int ret = pthread_mutex_lock(&t);
    return 0;
}

int test2()
{
    int ret = pthread_mutex_lock(&t);
    if (ret != 0)
        return 0;
    pthread_mutex_unlock(&t);
    return 0;
}

int test3()
{
    int ret = pthread_mutex_trylock(&t);
    return 0;
}

int test4()
{
    int ret = pthread_mutex_trylock(&t);
    if (ret != 0)
        return 0;
    pthread_mutex_unlock(&t);
    return 0;
}
