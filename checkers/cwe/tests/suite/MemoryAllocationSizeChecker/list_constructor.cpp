#include <iostream>
#include <list>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif  // EXTER_ATTACK

int EXTER_ATTACK foo(int n)
{
    std::list<int> list(n);
    return list.front();
}
