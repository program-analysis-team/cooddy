#include <deque>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(int n) {
    std::deque<int> d(n);
    d.push_front(13);
    d.push_back(25);
}
