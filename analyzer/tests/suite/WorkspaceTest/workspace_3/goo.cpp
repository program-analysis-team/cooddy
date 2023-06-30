#include <include_mism.h>

int main()
{
#ifdef MISMATCH
    int* ptr = new int;
    delete [] ptr;
#endif
    return 0;
}
