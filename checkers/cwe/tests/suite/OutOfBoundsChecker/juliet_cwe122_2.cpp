#include <cstdio>
#include <iostream>

void CWE122_Heap_Based_Buffer_Overflow__cpp_CWE129_fscanf_01_bad()
{
    int data;
    /* Initialize data */
    data = -1;
    /* POTENTIAL FLAW: Read data from the console using fscanf() */
    std::fscanf(stdin, "%d", &data);
    {
        int i;
        int* buffer = new int[10];
        /* initialize buffer */
        for (i = 0; i < 10; i++) {
            buffer[i] = 0;
        }
        /* POTENTIAL FLAW: Attempt to write to an index of the array that is above the upper bound
         * This code does check to see if the array index is negative */
        if (data >= 0) {
            buffer[data] = 1;
            /* Print the array values */
            for (i = 0; i < 10; i++) {
                std::cout << buffer[i];
            }
        } else {
            std::cout << "ERROR: Array index is negative.";
        }
        delete[] buffer;
    }
}
