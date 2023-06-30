#include <forward_list>
#include <iostream>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif  // EXTER_ATTACK

void EXTER_ATTACK foo(int n)
{
    std::forward_list<int> forwardList = {1, 2, 3};
    forwardList.resize(n);
    std::cout << forwardList.front() << '\n';
}
