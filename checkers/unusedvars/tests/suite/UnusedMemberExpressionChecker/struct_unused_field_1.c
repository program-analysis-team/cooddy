#define __STDC_WANT_LIB_EXT1__ 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint64_t *testPtr;
    uint64_t test;
} TEST_STRUCT_UNUSED_FIELD;

uint64_t TEST_StructWithUnusedField_1()
{
    TEST_STRUCT_UNUSED_FIELD p = {0, 0};

    p.testPtr = (uint64_t *)malloc(sizeof(uint64_t));

#ifdef __STDC_LIB_EXT1__
    (void)memset_s(p.testPtr, sizeof(uint64_t), 0, sizeof(uint64_t));
#endif
    return p.test;
}
