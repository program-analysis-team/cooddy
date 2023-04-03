// No unused variables
#include <stdio.h>
int foo(FILE *f){
    int aValue = 0;
    int bValue = 0;
    fwrite(&bValue, sizeof(bValue), 1, f);
    return aValue;
}
