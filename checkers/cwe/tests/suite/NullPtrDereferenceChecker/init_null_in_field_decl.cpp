
struct Foo {
    int* field = 0;
};

int func()
{
    Foo foo;
    return *foo.field;
}
