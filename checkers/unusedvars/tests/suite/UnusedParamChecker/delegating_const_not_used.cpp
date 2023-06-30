struct B {
    int par;
};

class A {
public:
    A(B& b, int c) : A(b.par) {};
    A(int val) : value(val){};

private:
    int value;
};
