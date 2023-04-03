class A {
    String ret = "abc";
    String bar(bool flag) {
        return flag ? null : ret;
    }
    void foo() {
        String ret1 = bar(false);
        String ret2 = bar(true);
        ret1.length();   // no error here
        ret2.length();   // error here
    }
}
