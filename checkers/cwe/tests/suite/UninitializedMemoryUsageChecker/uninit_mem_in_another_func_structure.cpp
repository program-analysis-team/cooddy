#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10
    typedef struct {
    int arr[MAX];
    int a;
    int b;
    int c;
} uninit_memory_access_007_s_001;

void uninit_memory_access_007_func_001(uninit_memory_access_007_s_001 *st)
{
    memset(st->arr, 0, 1);
    st->a = 1;
}
void uninit_memory_access_007_func_002(uninit_memory_access_007_s_001 *st)
{
    int temp = 0;
    int i;
    for (i = 0; i < MAX; i++) {
        temp += st->arr[i]; /*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
    }
}

void uninit_memory_access_007_func_003(uninit_memory_access_007_s_001 *st)
{
    st->b = 10;
    st->c = 20;
}

void uninit_memory_access_007_func_004(int num)
{
    int temp;
    if (num != 0) {
        temp = num;
    }
}
void uninit_memory_access_007()
{
    uninit_memory_access_007_s_001 st;
    uninit_memory_access_007_func_001(&st);
    uninit_memory_access_007_func_002(&st);
    uninit_memory_access_007_func_003(&st);
    uninit_memory_access_007_func_004(st.a);
}
