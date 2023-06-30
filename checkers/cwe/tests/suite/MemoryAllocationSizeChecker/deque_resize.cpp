#include <cstdlib>
#include <deque>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(char *str) {
    int size = std::atoi(str);
    std::deque<int> d(10);
    d.push_front(13);
    d.resize(size);
    d.push_back(25);
}
