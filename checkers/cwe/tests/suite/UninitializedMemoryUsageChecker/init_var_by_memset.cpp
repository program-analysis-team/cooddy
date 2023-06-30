#include <cstring>
int main()
{
    int testInteger;
    std::memset(&testInteger,1, sizeof(testInteger));
    int value = testInteger + 3;
    return 0;
}
