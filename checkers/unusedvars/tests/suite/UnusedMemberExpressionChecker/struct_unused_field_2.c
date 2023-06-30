#define __STDC_WANT_LIB_EXT1__ 1

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint64_t *testPtr;
    uint64_t test;
} TEST_STRUCT_UNUSED_FIELD;

uint64_t *TEST_StructWithUnusedField_2()
{
    TEST_STRUCT_UNUSED_FIELD p = {0};

    p.test = 0;
    return p.testPtr;
}
