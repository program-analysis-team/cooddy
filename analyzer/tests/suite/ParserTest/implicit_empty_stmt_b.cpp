template <bool UseAdd>
int FooImpl(int x, int y)
{
    //  There is template instantiation
    //  If condition is False => 'Then statement' is converted to empty statement
    if constexpr (UseAdd) {
        return x + y;
    } else {
        return x - y;
    }
}

int Foo(int x, int y)
{
    return FooImpl<false>(x, y);
}
