#include <string.h>

extern char tempDst[];

int TestCaseBad01(char* message)
{
    /* POTENTIAL FLAW:message */
    strncat(tempDst, message, strlen(message));
    return 0;
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
