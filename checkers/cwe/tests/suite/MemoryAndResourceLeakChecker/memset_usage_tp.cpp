#include <stdlib.h>
#include <cstdint>
int memset_s( void *dest, size_t destsz, int ch, size_t count );

void initFunc() {
    void* mem = malloc(0x1234);
    auto safeRet = memset_s(mem,0x1234,0,5);
    if (safeRet!= 0) {
        return; //MLC here
    }

    free(mem);

}
