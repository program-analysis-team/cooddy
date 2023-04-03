
struct Foo {
    int field;
};

int func(int* p, Foo* foo)
{
    return *p + foo->field;
}

int main()
{
    int a = 0;
    Foo f;
    f.field = 0;
    func(&a, &f);
}
