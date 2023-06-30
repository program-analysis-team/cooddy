#include <stdlib.h>

class BarObj {
public:
    BarObj() = default;
};

int main()
{
    BarObj *ptr = new BarObj();
    free(ptr);
    return 0;
}
