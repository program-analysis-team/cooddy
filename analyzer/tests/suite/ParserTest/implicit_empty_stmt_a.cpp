template <bool UseAdd>
int FooImpl(int x, int y)
{
    //  There isn't any template instantiation => 'then' statement isn't converted to empty statement
    if constexpr (UseAdd) {
        return x + y;
    } else {
        return x - y;
    }
}
