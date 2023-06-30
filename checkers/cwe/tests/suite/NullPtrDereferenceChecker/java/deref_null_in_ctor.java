class A {
    int xx;
    A(String x)
    {
        xx = x.length();
    }
};
class B {
    int main()
    {
        A a = new A(null);
    }
}
