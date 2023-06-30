#include <stdio.h>

void test_fopen_s(char* fName, int x)
{
    FILE* stream;
    errno_t err;
    err = fopen_s(&stream, fName, "r");
    if (err != 0) {
        return;
    }
    if (x) {
        return;
    }
    fclose(stream);
}

void test_freopen_s(char* fName, int x)
{
    FILE* stream;
    errno_t err;
    err = freopen_s(&stream, fName, "r", stdout);
    if (err != 0) {
        return;
    }
    if (x) {
        return;
    }
    fclose(stream);
}
