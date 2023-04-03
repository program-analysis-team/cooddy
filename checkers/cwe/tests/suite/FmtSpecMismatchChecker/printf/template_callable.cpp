struct Functor {
    void operator()(int val);
};

template <typename T>
void foo(T val)
{
    Functor f;
    f(val);
}
