#include <memory>
#include <cstring>

struct S {
    int a;
    int b[100];
    int c;
};

int foo(S* pp)
{
    auto p = std::make_unique<S>();
    memcpy(p.get(), pp, sizeof(S));  // no OOB here
    return 0;
}
