class A {
    Stirng[] m;
}
class B {
    int main()
    {
        A a = new A();
        A b;
        a.m = null;
        b = a;
        return b.m[2].length();
    }
}
