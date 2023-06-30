#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int test()
{
    char pPath[256];
    std::ifstream is ("test.txt");
    is.read(pPath,256);

    char* newPath = (char*)malloc(0x123);
    memccpy(newPath,pPath,4,5);
    execl(newPath, newPath, "1", "2", nullptr);

    char* newPath2 = (char*)malloc(0x1234);
    is.read(pPath,256);
    strcpy(newPath2,pPath);
    execl(newPath2, newPath2, "1", "2", nullptr);

    is.read(pPath,256);
    char* newPath3 = strdup(pPath);
    execl(newPath3, newPath3, "1", "2", nullptr);
    return 0;
}
