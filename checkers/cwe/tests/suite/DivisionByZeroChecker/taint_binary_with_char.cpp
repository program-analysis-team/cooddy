#include <fstream>

int foo()
{
    std::ifstream is("test.txt");
    char c;
    is.get(c);
    int res = c - 'A';
    return 100 / res;
}
