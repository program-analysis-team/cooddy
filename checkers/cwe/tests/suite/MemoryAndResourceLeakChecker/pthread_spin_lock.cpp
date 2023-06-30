#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tagMCS_LOCK_EXAMPLE2_S {
    pthread_spinlock_t mcslock;
    int count;
} MCS_LOCK_EXAMPLE2_S;

MCS_LOCK_EXAMPLE2_S g_mcsExamp;
static uint32_t g_mcsLockExample2Mem;
static uint32_t g_index[10] = {0};

#define mcs_spin_lock pthread_spin_lock
#define mcs_spin_unlock pthread_spin_unlock

void mcsLockExampleInit(void)
{
    int count = sizeof(g_index) / sizeof(g_index[0]);
    pthread_spin_init(&g_mcsExamp.mcslock, 0);
    g_mcsLockExample2Mem = 0;

    mcs_spin_lock(&g_mcsExamp.mcslock);
    for (int i = 0; i < count; i++) {
        g_index[i] == i;
    }
    mcs_spin_unlock(&g_mcsExamp.mcslock);
    return;
}

void mcsLockExample1(void)
{
    mcs_spin_lock(&g_mcsExamp.mcslock);
    g_mcsLockExample2Mem++;
    mcs_spin_unlock(&g_mcsExamp.mcslock);
    return;
}

void TEST_ResourceLeakSpinOther_4()
{
    int dindex = 3;
    int count = sizeof(g_index) / sizeof(g_index[0]);

    mcsLockExampleInit();
    mcsLockExample1();

    mcs_spin_lock(&g_mcsExamp.mcslock);
    for (int i = 0; i < count; i++) {
        if (g_index[i] == dindex) {
            // do something
            return;  // MLC error here
        }
    }
    mcs_spin_unlock(&g_mcsExamp.mcslock);
    return;
}
