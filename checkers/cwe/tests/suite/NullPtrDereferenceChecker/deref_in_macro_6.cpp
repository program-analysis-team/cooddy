#define BOO(x) (*((x)->p) = 0)

struct AAA {
    int* p;
};

void foo(AAA* a)
{
    a = nullptr;
    BOO(a);
}
