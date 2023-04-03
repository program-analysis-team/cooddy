#include <stdio.h>

const char* getString(bool flag)
{
    if (flag) {
        return nullptr;
    }
    return nullptr;
}

void outString(const char* myString)
{
    if (myString[0] != 0) {
        printf("%s\n", myString);
    }
}

int main()
{
    const char* myString = getString(false);
    outString(myString);
    return 0;
}
