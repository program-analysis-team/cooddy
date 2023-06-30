#include <vector>
int test(std::vector<int>& vec)
{
    int a = 0;
    for (auto& i : vec) {
        a + i;
    }
    return a;
}
