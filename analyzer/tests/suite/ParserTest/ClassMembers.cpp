class MyClass {
public:
    MyClass();

    int Foo(int value) const;

public:
    int myMemberA = 0;
    int myMemberB;

protected:
    int myMemberC;

private:
    int myMemberD;
};

MyClass::MyClass()
    //  It is initializer in the list
    : myMemberB(0) {
    //  It is initializer in the ctor body
    myMemberC = 0;

    //  myMemberD isn't initializerd at all
}

int MyClass::Foo(int value) const {
    int tmp = value;
    tmp += myMemberB;
    tmp += myMemberC;
    return tmp;
}
