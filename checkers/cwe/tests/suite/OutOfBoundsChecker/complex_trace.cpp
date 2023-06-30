#include <stdint.h>

struct Boo {
    char *field;
};

void some_func(uint32_t *idx);

void usage(Boo *arr, uint32_t idx)
{
    arr->field[idx];
}

uint32_t get_index()
{
    uint32_t idx;
    some_func(&idx);
    return idx;
}

void bar(Boo *arr)
{
    uint32_t idx = get_index();
    usage(arr, idx);
}

void foo()
{
    Boo boo;
    char arr[10];
    boo.field = arr;
    bar(&boo);
}
