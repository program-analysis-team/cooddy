namespace MyNamespaceA {

//  There are two global variables
int x = 0;
int y;

}  // namespace MyNamespaceA

namespace MyNamespaceB {

int Foo(int x, int y) { return x + y; }

int Goo(int a, int b) {
    static int x = a;
    int y = b;
    return x - y;
}

}  // namespace MyNamespaceB

namespace MyNamespaceC {

class MyClass {
public:
    MyClass();

    int Foo(int a);

public:
    int x = 0;
    int y;
};

}  // namespace MyNamespaceC

namespace MyNamespaceD {

class MyClass {
public:
    MyClass();

    int Foo(int a);

public:
    static const int x = 0;
    static int y;
};

int MyClass::y = 0;

}  // namespace MyNamespaceC
