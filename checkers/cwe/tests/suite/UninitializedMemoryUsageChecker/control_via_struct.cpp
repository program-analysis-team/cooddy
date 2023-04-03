struct A {
    int value;
};

void undefined(int *data, A *control);

int foo(int v)
{
    int data;

    A control;
    if (v == 1) {
        control.value = 1;
    } else {
        undefined(&data, &control);
    }

    if (control.value == 0) {
        return data;
    }
    return 0;
}
