#include <stdlib.h>

struct data {
    int* mem;
    bool oom;
};

void init_data(data& d)
{
    void* mem = malloc(sizeof(int));
    if (mem != 0)
        d.mem = (int*)mem;
    else
        d.mem = 0;
    d.oom = (mem == 0);
}

int main()
{
    data d;
    init_data(d);
    if (d.oom) {
        return 0;
    }
    return *d.mem;  // FP no error here
}
