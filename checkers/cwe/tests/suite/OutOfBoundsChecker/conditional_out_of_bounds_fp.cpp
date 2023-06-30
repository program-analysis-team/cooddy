#include <stdlib.h>

#include <cstring>

int someCondition();
void foo(int c, int x)
{
    int a = 7;
    int b;
    int arr[10] = {0};
    if (c == 10) {
        a = x;
    }

    memcpy(&b, &a, sizeof(int));
    if (c!=10)
        arr[b];  // no error here
}
