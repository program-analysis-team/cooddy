#include <iostream>
#include <unistd.h>

int test()
{
    char pPath[256];
    std::cin.getline (pPath,256);
    execl(pPath, pPath, "1", "2", nullptr);
    return 0;
}
