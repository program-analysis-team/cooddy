#include "std_testcase.h"

#include <wchar.h>

namespace CWE476_NULL_Pointer_Dereference__char_33 {

/* goodG2B() uses the GoodSource with the BadSink */
static void goodG2B() {
    char *data;
    char *&dataRef = data;
    /* FIX: Initialize data */
    data = "Good";
    {
        char *data = dataRef;
        /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
        /* printLine() checks for NULL, so we cannot use it here */
        printHexCharLine(data[0]);
    }
}

/* goodB2G() uses the BadSource with the GoodSink */
static void goodB2G() {
    char *data;
    char *&dataRef = data;
    /* POTENTIAL FLAW: Set data to NULL */
    data = NULL;
    {
        char *data = dataRef;
        /* FIX: Check for NULL before attempting to print data */
        if (data != NULL) {
            /* printLine() checks for NULL, so we cannot use it here */
            printHexCharLine(data[0]);
        } else {
            printLine("data is NULL");
        }
    }
}

void good() {
    goodG2B();
    goodB2G();
}
}  // namespace

using namespace CWE476_NULL_Pointer_Dereference__char_33; /* so that we can use good and bad easily */

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
    printLine("Calling good()...");
    good();
    printLine("Finished good()");
    return 0;
}
