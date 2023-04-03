
typedef struct {
    int idx;
    int* arr;
} SomeData;

int some_func(int param, SomeData* data);

int arr[10] = {0};

static int foo(int idx, int param, SomeData* arg)
{
    int localArr[10];
    SomeData data;
    data.idx = idx;
    data.arr = localArr;
    some_func(param, &data);
    int c = arr[idx];     // no error here
    int d = localArr[5];  // no error here

    some_func(param, arg);
    return arr[data.idx + arg->idx];  // error here
}
