class A {
    int foo(String value)
    {
        int b = value.length();
        return b;
    }
    int main()
    {
        return foo(null);
    }
}
