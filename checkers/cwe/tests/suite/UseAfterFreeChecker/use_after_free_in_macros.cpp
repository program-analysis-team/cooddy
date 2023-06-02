#include <iostream>
#include <stdlib.h>

#define EAM_CCDB_DestroyCfgOperationHandle(handle) { \
    free(handle);                                     \
    std::cout << handle[0]; }


int main()
{
    int *dbHandle = NULL;
    dbHandle = (int*) malloc(4*sizeof(int));
    EAM_CCDB_DestroyCfgOperationHandle(dbHandle);

    return 0;
}
