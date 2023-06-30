#include <stdio.h>

void CWE124_Buffer_Underwrite__CWE839_fscanf_01_bad()
{
    int data;
    /* Initialize data */
    data = -1;
    /* POTENTIAL FLAW: Read data from the console using fscanf() */
    fscanf(stdin, "%d", &data);
    {
        int i;
        int buffer[10] = {0};
        /* POTENTIAL FLAW: Attempt to write to an index of the array that is above the upper bound
         * This code does check to see if the array index is negative */
        if (data < 10) {
            buffer[data] = 1;
            /* Print the array values */
            for (i = 0; i < 10; i++) {
                printf("%d", buffer[i]);
            }
        } else {
            printf("ERROR: Array index is negative.");
        }
    }
}
