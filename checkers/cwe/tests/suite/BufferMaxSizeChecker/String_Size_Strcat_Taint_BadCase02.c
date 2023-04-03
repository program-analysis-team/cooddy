#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CMD_MAX 120

void TestCaseBad01(char* userinput)
{
    char cmd[CMD_MAX] = "/usr/bin/cat";

    strcat(cmd, userinput);
}

int main(int argc, char* argv[])
{
    if (argc > 1) {
        char* userinput = argv[1];
        TestCaseBad01(userinput);
    }
    return 0;
}
