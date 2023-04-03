
struct Boo {
    Boo& GetSome();
};

struct Foo {
    int GetNode(Boo&);
};

void func()
{
    auto makeBlend = [](Foo foo, Boo boo) { foo.GetNode(boo.GetSome()); };
}
