
struct Bar {
    void use_ptr()
    {
        *p = 10;
    }
    void init_ptr()
    {
        p = nullptr;
    }
    int* p;
};

void foo()
{
    Bar bar;
    bar.init_ptr();
    bar.use_ptr();
}
