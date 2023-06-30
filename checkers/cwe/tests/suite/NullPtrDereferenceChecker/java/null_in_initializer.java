class Foo {
    {
       field1 = "abc";
    }
    String field1;
};
class A {
    int main()
    {
        Foo foo = new Foo();
        return foo.field1.length();
    }
}
