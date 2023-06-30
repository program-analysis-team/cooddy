#include <stdlib.h>
struct list_head {
    list_head *next, *prev, *local;
};

void undefFun(list_head **);

list_head *some_alloc_with_leak(int size)
{
    list_head *sglChain = 0;
    list_head *sglInPos = 0;

    sglChain = (list_head *)malloc(0x1234);
    if (sglChain == nullptr) {
        return 0;
    }

    undefFun(&sglInPos);
    if (sglInPos == nullptr) {
        return nullptr;
    }
    return sglChain;
}
