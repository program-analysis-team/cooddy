struct A {
    int arr[5];
};

int main(int argc, char* argv[])
{
    struct A a;
    int* ptr = (int*)a.arr;
    *ptr = 42;
    return a.arr[0];
}
