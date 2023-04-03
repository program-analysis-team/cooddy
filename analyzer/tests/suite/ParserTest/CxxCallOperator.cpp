class MyClass {
public:
    MyClass& operator+=(int value) {
        myValue += myValue;
        return *this;
    }

    MyClass& operator-=(int value) {
        myValue -= myValue;
        return *this;
    }

private:
    int myValue = 0;
};

void Foo(MyClass& ref, int value) { ref += value; }

void Goo(MyClass& ref, int value) { ref -= value; }
