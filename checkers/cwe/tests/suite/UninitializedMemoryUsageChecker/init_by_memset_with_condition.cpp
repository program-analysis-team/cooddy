#include <memory.h>

int my_memset(int* p, int size)
{
    if (p == 0) {
        return -1;
    }
    memset(p, 0, size);
    return 0;
}

int func()
{
    int pLocalNodeId;
    if (my_memset(&pLocalNodeId, sizeof(pLocalNodeId))) {
        return 0;
    }
    return pLocalNodeId;  // no error here
}
