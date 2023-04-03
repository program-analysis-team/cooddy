
struct Foo {
    int a : 7;
    int b : 15;
    int c : 10;
};

int main()
{
    Foo foo;
    return foo.c;
}
