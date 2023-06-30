struct A {
    int a;
    int b;
};

A* foo()
{
    return nullptr;
}

int main()
{
    auto a = foo();
    if (!a || 3 > a->b) {
        return 1;
    }
    return 0;
}
