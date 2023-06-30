
struct Foo {
    void (*func)(int** p);
};

void func(Foo* foo)
{
    foo->func(0);
}

int main()
{
    func(0);
    return 0;
}
