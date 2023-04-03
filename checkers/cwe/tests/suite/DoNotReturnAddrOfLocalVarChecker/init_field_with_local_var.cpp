struct A {
    int *x;
    void foo()
    {
        int y;
        x = &y;
    }
};
