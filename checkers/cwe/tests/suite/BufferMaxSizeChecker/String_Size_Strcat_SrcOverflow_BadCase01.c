#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_MAX_SIZE 10

void TestCaseBad01(char* message)
{
    char tempDst[MSG_MAX_SIZE + 1] = {0x00};
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
