class Foo {
    String field;
}
class A {
    void func(Foo fo111, Foo fo222)
    {
        fo111.field = fo222.field;
    }

    int main()
    {
        Foo f11 = new Foo();
        Foo f22 = new Foo();
        f22.field = null;
        func(f11, f22);
        return f11.field.length();  // should be detect here
    }
}
