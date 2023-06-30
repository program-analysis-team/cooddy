#include <fstream>
#include <unistd.h>

int test()
{
    char pPath[256];
    std::ifstream is ("test.txt");
    is.read(pPath,256);
    execl(pPath, pPath, "1", "2", nullptr);
    is.readsome(pPath,256);
    execl(pPath, pPath, "1", "2", nullptr);
    return 0;
}
