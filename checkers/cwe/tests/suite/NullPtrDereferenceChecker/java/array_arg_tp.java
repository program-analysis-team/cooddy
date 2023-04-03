class A {
    int func(String[] a)
    {
        a[0].length();
    }

    int main()
    {
        String[] arr = {null, "abc"};
        return func(arr);
    }
}
