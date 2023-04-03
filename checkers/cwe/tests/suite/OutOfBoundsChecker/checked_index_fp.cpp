
using uint32_t = unsigned int;

struct S {
    uint32_t maxValue;
};

void foo(S* s, uint32_t val)
{
    int arr[9] = {0};
    uint32_t index = val < 10 ? val : 10;
    if (index >= s->maxValue) {
        return;
    }
    arr[index] = 0;
}
