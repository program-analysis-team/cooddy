#include <pthread.h>
#include <stdlib.h>

void int_lock1(int m);
void int_unlock1(int m);

void int_lock2(int m);
void int_unlock2(int m);

void test_custom_lock_fuctions()
{
    int mutex = 3;
    int_lock2(mutex);
    int_unlock2(mutex);  // no detect here
}
void test_custom_lock_fuctions2()
{
    int mutex = 2;
    int_lock1(mutex);
    int_unlock1(mutex);  // no detect here
}
void test_custom_lock_fuctions3()
{
    int mutex = 3;
    int_lock2(mutex);
    int_unlock1(mutex);  // detect here
}
