void FreeFunctionA() {}

void Foo() {}

class MyClass {
public:
    void NonStaticMethodA() {}

    void Foo() {}

    void StaticMethod() {}

public:
    MyClass() = default;

    explicit MyClass(int value) : myValue(value) {}

    MyClass(const MyClass&) = default;
    MyClass(MyClass&&) = delete;

    MyClass& operator=(const MyClass&) = default;
    MyClass& operator=(MyClass&&) = delete;

    ~MyClass() {}

private:
    int myValue;
};

void FooWithDeclarationOnly();
