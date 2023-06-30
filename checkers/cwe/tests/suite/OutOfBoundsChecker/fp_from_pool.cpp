#define NR_VCS 5

#define MAX_LOGIC_THREAD_NUM_FOR_POOL (80 * 2)

typedef enum tagDIRECT_READ_ROCACHE_STATE { DIRECT_READ_ROCACHE_LOCKED } DIRECT_READ_ROCACHE_STATE;

typedef struct tagDIRECT_READ_ROCACHE_STATE_S {
    DIRECT_READ_ROCACHE_STATE state;
} DIRECT_READ_ROCACHE_STATE_S;

int getDirectIoCnts(int *statsArray, int size)
{
    for (int index = 0; index < size; index++) {
        if (statsArray[index] != 0) {
            continue;
        }
    }
    return 0;
}

DIRECT_READ_ROCACHE_STATE_S g_directReadROCacheState[NR_VCS];

DIRECT_READ_ROCACHE_STATE getDirectReadROCacheState(int vcId)
{
    return g_directReadROCacheState[vcId].state;
}

typedef struct tagDIRECT_READ_COUNTER_S {
    int readTimes[MAX_LOGIC_THREAD_NUM_FOR_POOL];
    int writeTimes[MAX_LOGIC_THREAD_NUM_FOR_POOL];
    int readTotal[MAX_LOGIC_THREAD_NUM_FOR_POOL];
    int writeTotal[MAX_LOGIC_THREAD_NUM_FOR_POOL];
} DIRECT_READ_COUNTER_S;

void initDirectReadROCacheState(void)
{
    for (int vcId = 0; vcId < NR_VCS; vcId++) {
        g_directReadROCacheState[vcId].state = DIRECT_READ_ROCACHE_LOCKED;
    }
}

void showDirectReadStatus()
{
    DIRECT_READ_ROCACHE_STATE state;
    char *stateStr[] = {"LOCKED", "UNLOCKED", "LOCKING", "UNLOCKING", "BUTT"};
    DIRECT_READ_COUNTER_S *directReadCounter = 0;
    int vcId;

    initDirectReadROCacheState();

    for (vcId = 0; vcId < NR_VCS; vcId++) {
        getDirectIoCnts(directReadCounter->readTimes, MAX_LOGIC_THREAD_NUM_FOR_POOL);
        getDirectIoCnts(directReadCounter->writeTimes, MAX_LOGIC_THREAD_NUM_FOR_POOL);
        getDirectIoCnts(directReadCounter->readTotal, MAX_LOGIC_THREAD_NUM_FOR_POOL);
        getDirectIoCnts(directReadCounter->writeTotal, MAX_LOGIC_THREAD_NUM_FOR_POOL);

        state = getDirectReadROCacheState(vcId);

        stateStr[state];
    }

    return;
}
