
struct Foo {
    int* field1;
    int* field2;
};

int main(int v)
{
    int val = 10;
    Foo foo = {0, &val};
    int a = *foo.field2;
    int b = *foo.field1;
    return 0;
}
