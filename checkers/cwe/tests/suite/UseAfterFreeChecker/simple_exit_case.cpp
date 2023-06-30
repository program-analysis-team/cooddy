#include <stdlib.h>

void foo(int *&data)
{
    data = (int *)malloc(sizeof(int));
    if (data == nullptr) {
        exit(-1);
    }
    free(data);  // free
}

int main()
{
    int *data = (int *)malloc(sizeof(int));
    foo(data);
    return *data;  // UseAfterFree
}
