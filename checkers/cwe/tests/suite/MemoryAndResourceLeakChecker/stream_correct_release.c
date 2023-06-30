#include <stdio.h>

void test_fopen_2(char* fName, int x)
{
    FILE* f = fopen(fName, "r");
    if (!f) {
        return;
    }
    fclose(f);
}
