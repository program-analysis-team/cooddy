#include <cstdint>
#include <iostream>
#include <vector>

void foo()
{
    int32_t dim;
    size_t indexNum;
    std::cin >> dim;
    std::cin >> indexNum;
    std::vector<uint16_t> a(dim * indexNum);
}
