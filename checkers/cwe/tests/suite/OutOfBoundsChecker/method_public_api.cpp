class A {
    int private_foo_a(int a)
    {
        int arr[10] = {};
        return arr[a];  // no OOB here
    }

public:
    int public_foo_a(int a)
    {
        int arr[10] = {};
        return arr[a];  // OOB here
    }
};

struct B {
    int public_foo_b(int a)
    {
        int arr[10] = {};
        return arr[a];  // OOB here
    }

private:
    int private_foo_b(int a)
    {
        int arr[10] = {};
        return arr[a];  // no OOB here
    }
};
