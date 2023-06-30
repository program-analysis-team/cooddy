#include "std_testcase.h"

#include <wchar.h>

namespace CWE476_NULL_Pointer_Dereference__char_33 {

void bad() {
    char *data;
    char *&dataRef = data;
    /* POTENTIAL FLAW: Set data to NULL */
    data = __null;
    {
        char *data = dataRef;
        /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
        /* printLine() checks for NULL, so we cannot use it here */
        printHexCharLine(data[0]);
    }
}

}  // namespace

using namespace CWE476_NULL_Pointer_Dereference__char_33; /* so that we can use good and bad easily */

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
    printLine("Calling bad()...");
    bad();
    printLine("Finished bad()");
    return 0;
}
