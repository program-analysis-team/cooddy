#include <pthread.h>
#include <stdlib.h>

void int_alloc1(int m);
void int_free1(int m);

void int_alloc2(int m);
void int_free2(int m);

void test_custom_lock_fuctions()
{
    int mutex = 3;
    int_alloc2(mutex);
    int_free2(mutex);  // no detect here
}
void test_custom_lock_fuctions2()
{
    int mutex = 2;
    int_alloc1(mutex);
    int_free1(mutex);  // no detect here
}
void test_custom_lock_fuctions3()
{
    int mutex = 3;
    int_alloc2(mutex);
    int_free1(mutex);  // detect here
}
