#include <stdint.h>

typedef struct TEST_STRUCT_UNUSED_FIELD TEST_STRUCT_UNUSED_FIELD;

struct TEST_STRUCT_UNUSED_FIELD {
    void (*destroy)(void *data);
    uint64_t test;
};

void TEST_StructWithUnusedField_3(const TEST_STRUCT_UNUSED_FIELD *pp)
{
    pp->destroy(0);
}
