#include <vector>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif  // EXTER_ATTACK

void EXTER_ATTACK foo(int n)
{
    std::vector<int> v;
    v.resize(n);
    for (int k = 0; k < n; ++k) {
        v[k] = k;
    }
}
