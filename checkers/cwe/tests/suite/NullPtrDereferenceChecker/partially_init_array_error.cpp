struct Foo {
    int * p;
    int v;
    int array[5];
};
int main()
{
    Foo foo = {0};
    foo.array[1] = 1; //no error here
    return *foo.p; // error here
}
