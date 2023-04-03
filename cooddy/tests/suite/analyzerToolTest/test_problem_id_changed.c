// clang-format off
//COODDY_SUPPRESS PossibleInlineFunctionChecker
void test2()
{
    int* p1 = 0;

    *p1 = 10;     /* comment */

    int* p2 = 0;  // comment

    /* comment */ *p2 = 20;
}
//COODDY_SUPPRESS PossibleInlineFunctionChecker
void test1()
{
    int* /* comment */ p1 = 0;  // comment
    // comment
    *p1 = 10;
    int* p2 = 0;
    *p2 = 20;
}
//COODDY_SUPPRESS PossibleInlineFunctionChecker
void test3(int* z)
{
    //COODDY_SUPPRESS PossibleCyclesMergingChecker
    while (*z != 0) { z++; }
    //COODDY_SUPPRESS PossibleCyclesMergingChecker
    while (*z != 0) { z++; }
    while (*z != 0) { z++; }
}

int Test4(int N);
//COODDY_SUPPRESS PossibleInlineFunctionChecker ReturnConstantValueChecker
int Test4(int N)
{
	//return 0;
        // COODDY_SUPPRESS ReturnConstantValueChecker
	return 0;
}

// clang-format on
