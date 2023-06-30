#include <fstream>
#include <unistd.h>
#include <stdio.h>

int test()
{
    char pPath[256];
    std::ifstream is ("test.txt");
    is.read(pPath,256);
    char* newPath;
    sscanf(pPath,"",newPath);
    execl(newPath, newPath, "1", "2", nullptr);

    char* newPath2;
    is.read(pPath,256);
    snprintf(newPath2,5,"",pPath);
    execl(newPath2, newPath2, "1", "2", nullptr);

    char* newPath3;
    is.read(pPath,256);
    sprintf(newPath3,"",pPath);
    execl(newPath3, newPath3, "1", "2", nullptr);
    return 0;
}
