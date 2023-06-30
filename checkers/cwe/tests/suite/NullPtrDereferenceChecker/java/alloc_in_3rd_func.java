class A {
    public String buz() {
        return null;
    }
    public String bar() {
        return buz();
    }
    public void foo() {
        String s = "abc";
        s = bar();
        s.length();
    }
}
