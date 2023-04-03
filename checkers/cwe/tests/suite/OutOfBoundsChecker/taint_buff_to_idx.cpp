#include <cstdlib>

struct Foo {
    int size;
};

int arr[10];

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK bar(const char* str, int x)
{
    Foo foo = {std::atoi(str)};
    if (x > 10 && (foo.size < 10 && foo.size >= 0)) {
        return arr[foo.size];  // no error here
    }
    if (x > 10 || (foo.size < 10 && foo.size >= 0)) {
        return arr[foo.size];  // error here
    }
    return 0;
}
