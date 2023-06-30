
struct Data {
    int arr[10];
    int arr_size;
};

void taint(Data*);

void f(bool cond, Data* x)
{
    if (cond) {
        taint(x);
        return;
    }

    for (int i = 0; i < x->arr_size; i++) {
        x->arr[i];
    }
}
