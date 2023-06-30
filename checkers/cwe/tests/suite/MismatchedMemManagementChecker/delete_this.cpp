class A {
    public:
    A(int val) : a(val) {}

    void release() {
        delete this;
    }

private:
    int a;
};

int main()
{
    A *a = new A(6);

    a->release();

    delete new int(2);
    return 0;
}
