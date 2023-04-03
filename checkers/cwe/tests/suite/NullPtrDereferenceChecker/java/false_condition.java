class A {
    void test1()
    {
        String str = "abc";
        if (true) {
            str = null;
        }
        str.length(); // error
    }
    void test2()
    {
        String str = "abc";
        if (true)
            str = null;
        str.length(); // error
    }
    void test3()
    {
        String str = "abc";
        if (false) {
            str = null;
        }
        str.length(); // no error
    }
    void test4()
    {
        String str = "abc";
        if (false)
            str = "aaa";
        else
            str = null;
        str.length(); // error
    }
}
