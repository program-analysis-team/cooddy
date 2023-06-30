class Foo {
    String field;
}
class Bar {
    void func(Foo f)
    {
        f.field = null;
    }
    int main()
    {
        Foo foo = new Foo();
        func(foo);
        return foo.field.length();
    }
}
