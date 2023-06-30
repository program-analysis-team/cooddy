
class Foo {
    String myStr = nullptr;
};

class A {
    int main()
    {
        Foo foo = new Foo();
        return foo.myStr.length();
    }
}
