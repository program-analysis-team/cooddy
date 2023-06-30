#include <string.h>
#include <stdint.h>

#define ptrdiff_t unsigned long long
#define CACHE_LINE_SIZE 64
#define IO_PRIORITY_BUTT 12
#define PID_UTOP_BUTT 13

void *dpmm_malloc(int size, uint32_t pid, const char *func);

void *allocPermanentMemory(uint32_t size, const char *func, __int128_t pid)
{
    uint64_t allocSize = (uint64_t)size + CACHE_LINE_SIZE;
    ptrdiff_t addr;
    void *mem = NULL;

    mem = dpmm_malloc(allocSize, pid, func);
    addr = (ptrdiff_t)mem;

    if ((addr % CACHE_LINE_SIZE) != 0) {
        addr = addr - (addr % CACHE_LINE_SIZE) + CACHE_LINE_SIZE;
        mem = (void *)addr;
    }

    return mem;
}

static void* g_readPriStatisticMemory;

int32_t initGlobalMemory(void)
{
    uint32_t readPriStatisticSize = sizeof(uint64_t) * IO_PRIORITY_BUTT * PID_UTOP_BUTT;


    g_readPriStatisticMemory = allocPermanentMemory(readPriStatisticSize, __FUNCTION__, 100);
    if (g_readPriStatisticMemory == nullptr) {
        return -5;
    }

    memset(g_readPriStatisticMemory, 0, readPriStatisticSize);

    return 0;
}
