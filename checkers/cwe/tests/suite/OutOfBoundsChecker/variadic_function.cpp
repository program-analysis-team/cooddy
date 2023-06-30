#include <stdio.h>

void foo()
{
    int buffer[10] = {0};
    int idx1 = 0, idx2 = 0;
    int ret = fscanf(stdin, "%d%d", &idx1, &idx2);
    buffer[idx2];  // error here
    buffer[idx1];  // error here
    buffer[ret];   // error here
}
