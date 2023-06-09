/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE190_Integer_Overflow__short_max_postinc_07.c
Label Definition File: CWE190_Integer_Overflow.label.xml
Template File: sources-sinks-07.tmpl.c
*/
/*
 * @description
 * CWE: 190 Integer Overflow
 * BadSource: max Set data to the max value for short
 * GoodSource: Set data to a small, non-zero number (two)
 * Sinks: increment
 *    GoodSink: Ensure there will not be an overflow before incrementing data
 *    BadSink : Increment data, which can cause an overflow
 * Flow Variant: 07 Control flow: if(staticFive==5) and if(staticFive!=5)
 *
 * */

#include "std_testcase.h"

/* The variable below is not declared "const", but is never assigned
   any other value so a tool should be able to identify that reads of
   this will always give its initialized value. */
static int staticFive = 5;

#ifndef OMITBAD

void CWE190_Integer_Overflow__short_max_postinc_07_bad()
{
    short data;
    data = 0;
    if (staticFive == 5) {
        /* POTENTIAL FLAW: Use the maximum size of the data type */
        data = SHRT_MAX;
    }
    if (staticFive == 5) {
        {
            /* POTENTIAL FLAW: Incrementing data could cause an overflow */
            data++;
            short result = data;
            printIntLine(result);
        }
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodB2G1() - use badsource and goodsink by changing the second staticFive==5 to staticFive!=5 */
static void goodB2G1()
{
    short data;
    data = 0;
    if (staticFive == 5) {
        /* POTENTIAL FLAW: Use the maximum size of the data type */
        data = SHRT_MAX;
    }
    if (staticFive != 5) {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    } else {
        /* FIX: Add a check to prevent an overflow from occurring */
        if (data < SHRT_MAX) {
            data++;
            short result = data;
            printIntLine(result);
        } else {
            printLine("data value is too large to perform arithmetic safely.");
        }
    }
}

/* goodB2G2() - use badsource and goodsink by reversing the blocks in the second if */
static void goodB2G2()
{
    short data;
    data = 0;
    if (staticFive == 5) {
        /* POTENTIAL FLAW: Use the maximum size of the data type */
        data = SHRT_MAX;
    }
    if (staticFive == 5) {
        /* FIX: Add a check to prevent an overflow from occurring */
        if (data < SHRT_MAX) {
            data++;
            short result = data;
            printIntLine(result);
        } else {
            printLine("data value is too large to perform arithmetic safely.");
        }
    }
}

/* goodG2B1() - use goodsource and badsink by changing the first staticFive==5 to staticFive!=5 */
static void goodG2B1()
{
    short data;
    data = 0;
    if (staticFive != 5) {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    } else {
        /* FIX: Use a small, non-zero value that will not cause an overflow in the sinks */
        data = 2;
    }
    if (staticFive == 5) {
        {
            /* POTENTIAL FLAW: Incrementing data could cause an overflow */
            data++;
            short result = data;
            printIntLine(result);
        }
    }
}

/* goodG2B2() - use goodsource and badsink by reversing the blocks in the first if */
static void goodG2B2()
{
    short data;
    data = 0;
    if (staticFive == 5) {
        /* FIX: Use a small, non-zero value that will not cause an overflow in the sinks */
        data = 2;
    }
    if (staticFive == 5) {
        {
            /* POTENTIAL FLAW: Incrementing data could cause an overflow */
            data++;
            short result = data;
            printIntLine(result);
        }
    }
}

void CWE190_Integer_Overflow__short_max_postinc_07_good()
{
    goodB2G1();
    goodB2G2();
    goodG2B1();
    goodG2B2();
}

#endif /* OMITGOOD */

/* Below is the main(). It is only used when building this testcase on
   its own for testing or for building a binary to use in testing binary
   analysis tools. It is not used when compiling all the testcases as one
   application, which is how source code analysis tools are tested. */

#ifdef INCLUDEMAIN

int main(int argc, char* argv[])
{
    /* seed randomness */
    srand((unsigned)time(NULL));
#ifndef OMITGOOD
    printLine("Calling good()...");
    CWE190_Integer_Overflow__short_max_postinc_07_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE190_Integer_Overflow__short_max_postinc_07_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif
