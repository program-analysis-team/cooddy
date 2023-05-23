#include <pthread.h>

int some_lock(void* m)
{
    pthread_mutex_t* theMutex = (pthread_mutex_t*)m;
    if (theMutex == NULL) {
        return 1;
    }
    return pthread_mutex_lock(theMutex);
}