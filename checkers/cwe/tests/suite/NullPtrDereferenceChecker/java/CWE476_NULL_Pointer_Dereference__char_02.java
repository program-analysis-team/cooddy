class A {
    int CWE476_NULL_Pointer_Dereference__char_02_bad()
    {
        String data = "abc";
        if (true) {
            /* POTENTIAL FLAW: Set data to NULL */
            data = null;
        }
        if (true) {
            /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
            return data.length();
        }
        return 0;
    }
}
