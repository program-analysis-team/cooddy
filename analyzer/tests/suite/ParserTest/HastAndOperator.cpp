struct MyClass {
    MyClass() : myValue() {}
    explicit MyClass(int value) : myValue(value) {}

    MyClass(const MyClass&) = default;
    MyClass(MyClass&&) = default;

    ~MyClass() = default;

private:
    int myValue;
};
