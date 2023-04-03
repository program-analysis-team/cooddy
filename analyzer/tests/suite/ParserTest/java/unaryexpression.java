class Main {
    void foo(boolean b1, int x1)
    {
        boolean b2 = !b1;
        int x2 = ~x1;
        x1++;
        ++x1;
        x1--;
        --x1;
        int x3 = +x1;
        int x4 = -x1;
    }
}
