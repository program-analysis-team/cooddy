#include <pthread.h>

int foo();

struct AA {
    pthread_mutex_t lock;
};

void test_lock_global(struct AA *a)
{
    pthread_mutex_lock(&a->lock);
    if (foo()) {
        return;
    }
    pthread_mutex_unlock(&a->lock);
}
