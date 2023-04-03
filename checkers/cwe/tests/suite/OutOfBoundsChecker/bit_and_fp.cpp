void EXTER_ATTACK foo(unsigned int p)
{
    int arr[2];
    arr[p&1] = 0;  //
    arr[1&p] = 0;  // FP
}
