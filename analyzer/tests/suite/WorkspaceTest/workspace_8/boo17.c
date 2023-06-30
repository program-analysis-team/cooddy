#include <vector>
#include <optional>
int main()
{
    std::vector<int> test;
    std::optional<int> test2;
    int* ptr = nullptr;
    return ptr[test.size() + *test2];
}
