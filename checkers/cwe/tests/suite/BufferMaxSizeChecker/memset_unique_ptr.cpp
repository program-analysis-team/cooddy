#include <memory>
#include <string.h>
#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

class A {
    int arr[10];
};

class B {
public:
    std::unique_ptr<A> aptr;
    void EXTER_ATTACK foo() {
        auto dst = aptr.get(); //
        memset(dst, 0, sizeof(A));
    }
};
