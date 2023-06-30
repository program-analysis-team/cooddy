#include <vector>
int test(std::vector<int>& vec, int var)
{
    for (int a = var; auto& i : vec) {
        a + i;
    }
    return 0;
}
