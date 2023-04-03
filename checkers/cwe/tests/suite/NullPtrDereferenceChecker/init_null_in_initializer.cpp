
const int g_val = 10;

struct Foo {
    Foo() : field(0), field1(&g_val) {}
    const int* field = &g_val;
    const int* field1 = 0;
};

int func()
{
    Foo foo;
    int a = *foo.field1;  // no error here
    return *foo.field;    // error here
}
