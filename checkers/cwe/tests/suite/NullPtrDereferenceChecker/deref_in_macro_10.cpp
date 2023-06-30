void* malloc(unsigned);

#define MY_ALLOC(i) ((int*)malloc(i))

void foo()
{
    int x = *MY_ALLOC(0);
}
