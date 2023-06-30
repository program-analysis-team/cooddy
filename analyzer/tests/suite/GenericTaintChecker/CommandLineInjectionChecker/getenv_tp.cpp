#include <stdlib.h>
#include <unistd.h>

int test()
{
    char* pPath = getenv("EXE_PATH");
    execl(pPath, pPath, "1", "2", nullptr);
    return 0;
}
