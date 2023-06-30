#include <vector>

int main()
{
    std::vector<int> test;
    int* ptr = nullptr;
    return ptr[test.size()];
}
