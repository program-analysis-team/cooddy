
#include <limits>

int func()
{
    static const std::size_t bits = std::numeric_limits<long long>::digits;
    return 20 / bits;
}
