#include <cstdlib>

struct Foo {
    int size;
};

int bar2(int idx)
{
    int arr[10] = {0};
    return arr[idx];
}

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK bar(Foo* foo, int x)
{
    if (x < 10 && foo->size < 10) {
        return bar2(foo->size);  // no error here
    }
    if (x > 10 || foo->size < 10) {
        return bar2(foo->size);  // error here
    }
    return 0;
}
