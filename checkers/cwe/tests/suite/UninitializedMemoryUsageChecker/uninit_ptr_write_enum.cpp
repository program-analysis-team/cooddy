
typedef enum { ZERO, ONE, TWO } values;

void uninit_pointer_015_func_001(int *ptr)
{
    values val = ONE;
    *ptr = val; /*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}

void uninit_pointer_015()
{
    int *ptr;
    uninit_pointer_015_func_001(ptr);
}
