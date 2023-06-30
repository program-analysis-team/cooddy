class B {
public:
    B(int a) : myA(a) {}
    int myA;
};

class A : public B {
public:
    A(int a) : B(a){};
};
