class A {
    int func()
    {
        return 0;
    }

    int ReadSize()
    {
        return func();
    }

    int main()
    {
        int a = ReadSize();
        int b = 1000 / a;
        return 0;
    }
}
