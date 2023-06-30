template <bool UseAdd>
int FooImpl(int x, int y)
{
    //  There is template instantiation but there isn't any empty statement
    //  constexpr if condition is True => clang just throws to /dev/null 'else statement'
    if constexpr (UseAdd) {
        return x + y;
    } else {
        return x - y;
    }
}

int Foo(int x, int y)
{
    return FooImpl<true>(x, y);
}
