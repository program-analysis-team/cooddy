#include <stdio.h>

void test_open_memstream(int x)
{
    char* bp;
    size_t size;
    FILE* stream;
    stream = open_memstream(&bp, &size);
    if (stream == NULL) {
        return;
    }
    if (x) {
        return;
    }
    fclose(stream);
}

void test_fmemopen(int x)
{
    FILE* stream;
    stream = fmemopen(0, 0, "r");
    if (stream == NULL) {
        return;
    }
    if (x) {
        return;
    }
    fclose(stream);
}

void test_fopen(char* fName, int x)
{
    FILE* fp;
    if ((fp = fopen(fName, "w")) == NULL) {
        return;
    }
    if (x) {
        return;
    }
    fclose(fp);
}
