class Foo {
    String field;
};
class A {
    int main()
    {
        Foo foo11 = new Foo(), foo22 = new Foo();
        foo22.field = null;
        foo11.field = foo22.field;
        return foo11.field.length();  // should be detect here
    }
}
