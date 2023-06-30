#include <string.h>

int main()
{
    double v;
    double r = 0;
    memcpy(&v, &r, sizeof(r));
    double c = v + 1;
    return 0;
}
