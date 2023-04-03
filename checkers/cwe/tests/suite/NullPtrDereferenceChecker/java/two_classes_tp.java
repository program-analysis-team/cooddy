class B {
    void fff()
    {
        A a = new A();
        String c = null;
        a.foo(c);
    }
}

class A {
    int foo(String x)
    {
        return x.length();  // NPD here
    }

    public void bar()
    {
        String a = null;
        //this.foo(a);
    }
}
