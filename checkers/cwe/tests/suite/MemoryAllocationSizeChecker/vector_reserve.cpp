#include <cstddef>
#include <vector>

std::size_t AAA();

void foo()
{
    std::vector<int> v;
    auto n_dim = AAA();
    v.reserve(n_dim);
    for (int k = 0; k < n_dim; ++k) {
        v.push_back(k);
    }
}
