#define ARRAY_SIZE 10

extern "C" int some_func();

int main()
{
    int arr[ARRAY_SIZE] = {0};
    return arr[some_func()];  // out of bounds
}
