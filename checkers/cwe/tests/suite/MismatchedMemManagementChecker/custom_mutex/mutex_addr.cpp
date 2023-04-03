#include <pthread.h>
#include <stdlib.h>

class my_mutex {};

void my_mutext_lock1(my_mutex* m);
void my_mutext_unlock1(my_mutex* m);

void my_mutext_lock2(my_mutex* m);
void my_mutext_unlock2(my_mutex* m);

my_mutex* GetMyMutex();
void test_custom_lock_fuctions()
{
    my_mutex mutex;
    my_mutext_lock2(&mutex);
    my_mutext_unlock2(&mutex);  // no detect here
}
void test_custom_lock_fuctions2()
{
    my_mutex mutex;
    my_mutext_lock1(&mutex);
    my_mutext_unlock1(&mutex);  // no detect here
}
void test_custom_lock_fuctions3()
{
    my_mutex mutex;
    my_mutext_lock2(&mutex);
    my_mutext_unlock1(&mutex);  // detect here
}
