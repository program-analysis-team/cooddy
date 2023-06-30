/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: 释放指针后置空
 * positive:true
 * Author: kwx1121955
 * Create: 2021年9月13日
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct cacheEntry_fo {
    /* data */
} cacheEntry;

typedef struct bitMapInfo_fo {
    cacheEntry *cacheEntryArr;
    int cacheEntryArrsize;
    /* data */
} bitMapInfo;

cacheEntry *use_check_test_malloc_error(int size)
{
    return (cacheEntry *)malloc(sizeof(cacheEntry) * size);
};
void use_check_test_free_error(void *ptr)
{
    free(ptr);
};

void free_ptr_set_NULL_error(bitMapInfo *bitinfo)
{
    bitinfo->cacheEntryArrsize = 1;
    bitinfo->cacheEntryArr = use_check_test_malloc_error(bitinfo->cacheEntryArrsize);
    if (bitinfo->cacheEntryArr == NULL) {
        return;
    }
    use_check_test_free_error(bitinfo->cacheEntryArr);
    // bitinfo->cacheEntryArr = NULL;
    bitinfo->cacheEntryArrsize = 0;
    free(bitinfo);
}
