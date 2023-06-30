struct Boo {
    int v;
};

struct Foo {
    Boo c;
    int * p;
    int v;
    int array[5];
};
int main()
{
    Foo foo = {{0}};
    foo.array[3] = 1; // no error here
    return foo.array[2]; // no error here
}
