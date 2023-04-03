class A {
    int func(String[] a)
    {
        int res = 0;
        for(int i = 0; i < 2; i++) {
            res += a[i].length();
        }
        return res;
    }

    int main()
    {
        String[] arr = {null, "abc"};
        return func(arr);
    }
}
