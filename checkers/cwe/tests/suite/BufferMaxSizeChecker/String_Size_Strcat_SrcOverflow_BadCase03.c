#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char tempDst[32];

void TestCaseBad01(char* message)
{
    strcat(tempDst, message);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return 0;
    }
    char* message = argv[1];
    TestCaseBad01(message);
    return 0;
}
