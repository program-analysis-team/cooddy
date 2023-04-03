#include "std_testcase.h"

static void good1()
{
    {
        twoIntsStruct *twoIntsStructPointer = NULL;
        /* FIX: Use && in the if statement so that if the left side of the expression fails then
         * the right side will not be evaluated */
        if ((twoIntsStructPointer != NULL) && (twoIntsStructPointer->intOne == 5))
        {
            printLine("intOne == 5");
        }
    }
}

void CWE476_NULL_Pointer_Dereference__binary_if_01_good()
{
    good1();
}
