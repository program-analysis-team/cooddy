#include <stdio.h>
#include <stdlib.h>

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_01_bad()
{
    int data;
    /* Initialize data */
    data = -1;
    {
        char inputBuffer[CHAR_ARRAY_SIZE] = "";
        /* POTENTIAL FLAW: Read data from the console using fgets() */
        if (fgets(inputBuffer, CHAR_ARRAY_SIZE, stdin) != NULL) {
            /* Convert to int */
            data = atoi(inputBuffer);
        } else {
            printf("fgets() failed.");
        }
    }
    {
        int i;
        int buffer[10] = {0};
        /* POTENTIAL FLAW: Attempt to write to an index of the array that is above the upper bound
         * This code does check to see if the array index is negative */
        if (data >= 0) {
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
