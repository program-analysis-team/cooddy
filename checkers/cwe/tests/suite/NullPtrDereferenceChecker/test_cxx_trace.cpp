
struct Foo {
    void use_ptr(int a, int* b)
    {
        *b = 10;
    }
    int* get_ptr()
    {
        return nullptr;
    }
};

void bar()
{
    Foo foo;
    auto p = foo.get_ptr();
    foo.use_ptr(1, p);
}
