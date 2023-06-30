#include <forward_list>
#include <iostream>
#include <string>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif  // EXTER_ATTACK

void EXTER_ATTACK foo(int n)
{
    std::forward_list<std::string> forwardList(n, "YES");
    std::cout << forwardList.front() << '\n';
}
