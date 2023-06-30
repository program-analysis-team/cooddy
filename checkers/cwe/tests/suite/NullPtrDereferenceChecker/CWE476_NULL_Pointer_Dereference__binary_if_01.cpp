#include "std_testcase.h"

void CWE476_NULL_Pointer_Dereference__binary_if_01_bad()
{
    {
        twoIntsStruct *twoIntsStructPointer = NULL;
        /* FLAW: Using a single & in the if statement will cause both sides of the expression to be evaluated
         * thus causing a NPD */
        if ((twoIntsStructPointer != NULL) & (twoIntsStructPointer->intOne == 5))
        {
            return;
        }
    }
}

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
    printLine("Calling bad()...");
    CWE476_NULL_Pointer_Dereference__binary_if_01_bad();
    printLine("Finished bad()");
    return 0;
}
