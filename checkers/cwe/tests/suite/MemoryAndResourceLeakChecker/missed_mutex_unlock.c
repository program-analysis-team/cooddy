#include <pthread.h>

int foo();
pthread_mutex_t lock;

void test_global_mutex()
{
    pthread_mutex_lock(&lock);
    if (foo()) {
        return;
    }
    pthread_mutex_unlock(&lock);
}
