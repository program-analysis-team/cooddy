class A {
    int foo_wo_defenition();

    int private_foo_a(int a)
    {
        return 0;
    }

public:
    int public_foo_a(int a)
    {
        return 0;
    }
};

struct B {
    int public_foo_b(int a)
    {
        return 0;
    }

private:
    int private_foo_b(int a)
    {
        return 0;
    }
};

class C {
    class D {
        int foo_d(int a)
        {
            return 0;
        }
        int foo_wo_defenition();
    };
};
