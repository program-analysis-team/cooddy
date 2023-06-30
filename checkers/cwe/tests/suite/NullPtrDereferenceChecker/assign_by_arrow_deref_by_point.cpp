
struct Foo {
    int* field;
};

void func(Foo* foo)
{
    foo->field = 0;
}

int main()
{
    Foo foo;
    func(&foo);
    return *foo.field;
}
