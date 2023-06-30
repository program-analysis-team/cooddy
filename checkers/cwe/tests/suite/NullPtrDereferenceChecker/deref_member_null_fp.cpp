
struct Foo {
    int field;
};

int main()
{
    return (long long)&(((Foo*)0)->field);
}
