#include <stdlib.h>

#include <cstdint>

struct A {
    int* buf;
    A* a;
};

//some undef func, with unknown behaviour
void dplwt_task_run(int stLwtId, void *pvFunArg, int uiPid, const char *pcFuncName);

int32_t dplwt_task_create(int *lwt_id, int entry_func, void *arg, int *attr,
                          uint32_t uiPid, const char *pcFuncName);


void initFunc()
{
    A* param;
    param->a->buf = (int*)malloc(0x1234);
    dplwt_task_run(4, (void *)param->a, 123, "_function_name_");
    int lwtId;
    dplwt_task_create(&lwtId, 4, (void *) param, &lwtId, 1234, "_function_name_");
}
