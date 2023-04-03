
struct Foo {
    template <class T>
    const T func() const
    {
        return x ? 0 : 1;
    }
    bool x;  // no error here
};

int main()
{
    Foo foo;
    foo.func<int>();
    foo.func<short>();
    return 0;
}
