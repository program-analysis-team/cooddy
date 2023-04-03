
using uint32_t = unsigned int;

void foo(uint32_t index)
{
    int arr[9] = {0};
    if (index >= 10) {
        return;
    }
    arr[index] = 0;
}
