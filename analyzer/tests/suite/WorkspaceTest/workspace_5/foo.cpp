#include <vector>

struct A {
    int b;
};
int test();
namespace b {
class B {
public:
    B()
    {
        c = 1;
    };
    B(B& cp)
    {
        c = cp.c;
    }
    int get_c()
    {
        return c;
    }

private:
    int c;
};
}  // namespace b
int main_foo_1234()
{
    std::vector<A> p;
    b::B b;
    return p.size() + b.get_c() + test();
}
