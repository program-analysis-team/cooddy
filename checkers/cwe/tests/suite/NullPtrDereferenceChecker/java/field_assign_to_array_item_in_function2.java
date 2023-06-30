class Foo {
    String field = null;
}
class A {
    void func(Foo fo111, String[] fo222)
    {
        fo222[1] = fo111.field;
    }

    int main()
    {
        Foo f11;
        String[] f22 = new String[10];
        func(f11, f22);
        return f22[1].length();  // should be detected here
    }
}
