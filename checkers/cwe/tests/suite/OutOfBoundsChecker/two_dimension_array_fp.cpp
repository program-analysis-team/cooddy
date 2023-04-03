#include <stdint.h>

typedef struct tagDOP_MSG_DEBUG_STATIS_S {
    uint64_t allocChainNum;
    uint64_t freeChainNum;
    uint64_t allocChainNum1;
    uint64_t freeChainNum1;
    uint64_t allocChainNum2;
    uint64_t freeChainNum2;
} DOP_MSG_DEBUG_STATIS_S;

typedef enum tagDOP_MSG_STATIS_TYPE_E {
    DOP_MSG_STATIS_TYPE_SELF = 0,
    DOP_MSG_STATIS_TYPE_XRB,
    DOP_MSG_STATIS_TYPE_KVS,

    DOP_MSG_STATIS_TYPE_BUTT
} DOP_MSG_STATIS_TYPE_E;

DOP_MSG_DEBUG_STATIS_S arr[40][3];

extern void atomic64_add(uint64_t delta, uint64_t* ptr);

void foo(uint32_t type, uint32_t src)
{
    if (type >= 40 || src == 3) {
        return;
    }
    atomic64_add(src, &arr[type][src].allocChainNum);
}
