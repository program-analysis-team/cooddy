#include <iostream>
#include <stdlib.h>
#include <cstring>

#define EAM_CCDB_DestroyCfgOperationHandle(a, b) { \
    free(a);                                       \
    memcpy(a, b, sizeof(int)); }


int main()
{
    int *dbHandle1 = NULL;
    int *dbHandle2 = NULL;
    dbHandle1 = (int*) malloc(4*sizeof(int));
    dbHandle2 = (int*) malloc(4*sizeof(int));
    EAM_CCDB_DestroyCfgOperationHandle(dbHandle1, dbHandle2);

    return 0;
}
