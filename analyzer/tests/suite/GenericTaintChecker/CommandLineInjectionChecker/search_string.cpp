#include <fstream>
#include <unistd.h>
#include <string.h>

int test()
{
    char pPath[256];
    std::ifstream is ("test.txt");
    is.read(pPath,256);

    char* newPath = (char*)memchr(pPath, 5,5);
    execl(newPath, newPath, "1", "2", nullptr);

    is.read(pPath,256);
    char* newPath2 = strchr(pPath,5);
    execl(newPath2, newPath2, "1", "2", nullptr);

    is.read(pPath,256);
    char* newPath3 = strrchr(pPath,5);
    execl(newPath3, newPath3, "1", "2", nullptr);

    is.read(pPath,256);
    char* newPath4 = strncpy(pPath,pPath,5);
    execl(newPath4, newPath4, "1", "2", nullptr);

    is.read(pPath,256);
    char* newPath5 = strncat(pPath,pPath,5);
    execl(newPath5, newPath5, "1", "2", nullptr);
    return 0;
}
