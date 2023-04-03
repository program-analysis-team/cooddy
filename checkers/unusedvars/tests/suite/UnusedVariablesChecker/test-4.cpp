#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// No unused variables
int foo(FILE *f)
{
    int *aValue = (int *)malloc(sizeof(int));
    int bValue = 0;
    memset(aValue, 1, sizeof(int));
    fwrite(&bValue, sizeof(bValue), 1, f);
    return 0;
}
