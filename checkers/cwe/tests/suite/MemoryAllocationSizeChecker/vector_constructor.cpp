#include <cstdio>
#include <vector>

size_t AAA();

void foo()
{
    int size = AAA();
    std::vector<int> a(size);
}
