#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct {
    int m1;
    int m2;
    int m3;
    int m4;
} st1;

typedef struct {
    int aa1;
    int aa2;
    st1 *stNode;
    int aa3;
    int aa4;
} struct_all;

int FreeMem(void *pAddr)
{
    int ret = 0;
    free(pAddr);
    return ret;
}

void func3(struct_all *st_all)
{
    if (st_all->stNode == NULL) {
        printf("st_all->stNode is NULL.\n");
        return;
    }
    // FreeMem(st_all->stNode);
    return;
}

int func2(struct_all *st_all)
{
    func3(st_all);
    return 0;
}

int func1(struct_all *st_all)
{
    (void)func2(st_all);
    return 0;
}

int main()
{
    struct_all st_all = {0};
    st_all.stNode = (st1 *)malloc(sizeof(st1));

    if (st_all.stNode == NULL) {
        return -1;
    }

    (void)func1(&st_all);

    return 0;
}
