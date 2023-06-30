class A {
    public int foo() {
        String ret1 = null;
        if ((ret1 != null) & (ret1.length() == 5))
        {
            return 0; // error here
        }
        return 1;
    }
}
