#include <cstdio>
#include <vector>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

struct AStruct {};
void EXTER_ATTACK foo(int size) {
    std::vector<AStruct> a(size);
    std::vector<AStruct> b(a);
}
