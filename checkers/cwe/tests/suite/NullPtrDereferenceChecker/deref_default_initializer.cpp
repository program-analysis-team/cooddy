
class Foo {
public:
    int myVal;
    int* myPtr = nullptr;

    Foo() : myVal(10) {}
};

int main()
{
    Foo foo;
    return *foo.myPtr;
}
