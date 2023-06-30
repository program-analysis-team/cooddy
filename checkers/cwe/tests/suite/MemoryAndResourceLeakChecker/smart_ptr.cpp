#include <memory>

void foo()
{
    std::unique_ptr<int> p1(new int(42));
    std::shared_ptr<int> p2(new int(43));
}
