#include <string.h>

void BadCase01(char **argv)
{
    char testName[256] = { 0 };


    strncpy(testName, argv[1], 256);//error
}
