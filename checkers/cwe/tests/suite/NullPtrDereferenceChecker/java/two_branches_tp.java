class A {
    int foo(String x)
    {
        if (x != null) {
            return x.length();
        }
        return x.length();  // NPD here
    }

    void bar()
    {
        foo(null);
    }
}
