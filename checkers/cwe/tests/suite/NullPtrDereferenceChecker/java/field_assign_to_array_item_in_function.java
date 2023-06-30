class Foo {
    String field;
}
class A {
    void func(Foo fo111, String[] fo222)
    {
        fo111.field = fo222[1];
    }

    int main()
    {
        Foo f11;
        String[] f22 = new String[10];
        f22[1] = null;
        func(f11, f22);
        return f11.field.length();  // should be detected here
    }
}
