#include <stdint.h>

typedef struct retSTAT {
    uint64_t ret;
    uint32_t size;
} RETURN_ST;

#define MACRO_2(statParam)                    \
    RETURN_ST* param = (RETURN_ST*)statParam; \
    return (param->ret);

#define MACRO_1(ID, TIME, KEY)     \
    do {                           \
        MACRO_2((uint64_t*)(KEY)); \
    } while (0)

uint64_t foo()
{
    uint64_t time = 0;
    uint32_t delay = 0;
    MACRO_1(0, &time, &delay);
}
