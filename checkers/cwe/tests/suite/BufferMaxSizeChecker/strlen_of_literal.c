#include "string.h"

void TestCase01()
{
    const char* source = "Twenty characters!!!";
    char dest[10] = {0};
    strncpy(dest, source, strlen(source));  // source > dest
}

void TestCase02()
{
    const char* source = "Twenty characters!!!";
    auto len = strlen(source);
    char dest[21] = {0};
    strncpy(dest, source, len);  // OK
}
