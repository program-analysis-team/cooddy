
struct Foo {
    int x;  // no error here
};

void func() {}

int main()
{
    int v = 10;
    try {
        func();
    } catch (const Foo& foo) {
        v = foo.x;
    }
    v = 5;
    return v;
}
