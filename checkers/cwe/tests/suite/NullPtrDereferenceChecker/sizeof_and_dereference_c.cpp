int main()
{
    char *pointer = 0;
    int vInt = 0;
    if (vInt == 1) {                  //  The code is unreachable
        auto x = sizeof(pointer[0]);  //	Dereference is used in the compile time expression
    }

    pointer[0] = 0;  //	Actual null pointer dereference we should detect
}
