#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void TestCaseBad01()
{
    char DestStr[16] = "Enter name:";
    char SrcStr[] = "my name is great";
    strcat(DestStr, SrcStr);  // error here
}

int main(void)
{
    TestCaseBad01();
    return 0;
}
