#include <pthread.h>
#include <stdlib.h>

class my_mutex;

void my_mutext_lock1(my_mutex* m);
void my_mutext_unlock1(my_mutex* m);

void my_mutext_lock2(my_mutex* m);
void my_mutext_unlock2(my_mutex* m);

my_mutex* GetMyMutex();

void LockWrap1(my_mutex *m) {
    my_mutext_lock1(m);
}
void LockWrap2(my_mutex *m) {
    my_mutext_lock2(m);
}
void UnlockWrap1(my_mutex *m) {
    my_mutext_unlock1(m);
}
void UnlockWrap2(my_mutex *m) {
    my_mutext_unlock2(m);
}
void test_custom_lock_fuctions()
{
    my_mutex* mutex = GetMyMutex();
    LockWrap1(mutex);
    UnlockWrap1(mutex);  // no detect here
}
void test_custom_lock_fuctions2()
{
    my_mutex* mutex = GetMyMutex();
    LockWrap2(mutex);
    UnlockWrap2(mutex);  // no detect here
}
void test_custom_lock_fuctions3()
{
    my_mutex* mutex = GetMyMutex();
    LockWrap1(mutex);
    UnlockWrap2(mutex);  // detect here
}
