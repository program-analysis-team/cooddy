#include <stdint.h>

int MSC_GetAllClusterId(int *);

int32_t PlogFillClusterInfo(int *info, int *clusterInfo)
{
    for (uint32_t i = 0; i < *clusterInfo; i++) {
        (void)info[i];
    }
    return 0;
}

int32_t PlogGetClusterSiteInfoProc()
{
    int x;
    MSC_GetAllClusterId(&x);
    int array[10];
    return PlogFillClusterInfo(array, &x);
}
