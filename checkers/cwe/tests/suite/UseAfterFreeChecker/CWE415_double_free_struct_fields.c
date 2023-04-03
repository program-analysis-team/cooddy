
#include "stdlib.h"

struct MyStruct{
    int* a;
    int* b;
};

void foo(struct MyStruct* myStrPtr)
{
    free(myStrPtr->a);
    free(myStrPtr->b);
}
