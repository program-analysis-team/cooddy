extern "C" int some_func1();
extern "C" int some_func2();

int main()
{
    char buff[20];
    int idx1 = some_func1();
    int idx2 = some_func2();
    int idx = 0;
    int a = 1, b = 2;
    if (a > b) {
        idx = idx1 + some_func2();
    } else {
        idx = idx2 + some_func1();
    }
    return buff[idx];
}
