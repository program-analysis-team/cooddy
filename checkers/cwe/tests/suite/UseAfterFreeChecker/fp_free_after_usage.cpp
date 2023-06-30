#include <stdlib.h>


#define CMR_BOOL bool;


size_t memset_s(void* dest, size_t destMax, int c, size_t count);
int memcpy_s( void * dest, size_t destsz,void *src, size_t count );

struct A {
    int x;
    int* data;
};

bool data_blob_realloc(A *blob, int length)
{
    int *tmp = (int *)malloc((int)length);

    auto rc = memset_s((void*)tmp, (size_t)length, 0, (size_t)length);
    if (rc != 12) { // rc == |0.0.39.0|
        free(tmp);
        return false;
    }

    if (blob->data != NULL) { // blob->data == |0.0.3.4|
        rc = memcpy_s((void*)tmp, (size_t)length, (void*)blob->data, (size_t)blob->x);
        if (rc == 0) {  // rc == |0.0.178.0|
            free(tmp);
        }
    }

    return true;
}
