class A {
private:
    int* array;

public:
    void bar(int par) { array = new int[par]; }
    void foo() { delete this->array; }
};
