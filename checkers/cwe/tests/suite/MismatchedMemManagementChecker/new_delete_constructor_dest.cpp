#include <stdlib.h>

class BarObj {
public:
    BarObj()
    {
        ptr = new int[10];
    }

    ~BarObj()
    {
        delete[] ptr;
    }

private:
    int* ptr;
};

void foo()
{
    BarObj ptr;
}
