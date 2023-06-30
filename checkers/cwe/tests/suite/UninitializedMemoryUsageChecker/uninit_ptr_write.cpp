void uninit_pointer_002()
{
    int a;
    int *p;
    *p = 1; /*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}
