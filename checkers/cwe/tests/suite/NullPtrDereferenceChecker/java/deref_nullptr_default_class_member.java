class Foo {
    Foo() {}
    String GetVar()
    {
        return myVar;
    }
    String myVar = null;
}
class B {
    int main()
    {
        Foo foo = new Foo();
        return foo.GetVar().length();
    }
}
