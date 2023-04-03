class A {
    int func(String[] a)
    {
        int res = 0;
        for(String s : a) {
            res += s.length();
        }
        return res;
    }

    int main()
    {
        String[] arr = {null, "abc"};
        return func(arr);
    }
}
