struct Foo {
    int * p;
    int v;
};
int main()
{
    Foo foo = {0};
    if (foo.v == 0)
        foo.p = &foo.v;
    return *foo.p; // no error here
}
