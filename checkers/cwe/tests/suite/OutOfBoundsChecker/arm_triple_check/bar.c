
#ifndef __ARM_PCS_VFP

int g_arr[10];

void foo()
{
    g_arr[11];  // OOB error
}

#endif
