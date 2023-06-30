#include <stdlib.h>

#define SQLITE_ROW 100

int getValue(int var)
{
    return var;
}

int main()
{
    char **ppChar = 0;
    int vInt = 0;
    int nElem = 0;
    int nAlloc = 0;

    char vChar = 'q';
    char *pChar = &vChar;

    if (getValue(0)) {
        return 0;
    }

    while (getValue(vInt) == SQLITE_ROW) {
        if (nElem >= nAlloc - 2) {
            nAlloc = nAlloc * 2 + nElem + 10;
            ppChar = (char **)realloc(ppChar, nAlloc * sizeof(ppChar[0]));
            if (ppChar == 0) {
                return 1;
            }
        }
        ppChar[++nElem] = pChar;
        vInt++;
    }

    ppChar[0] = 0;
}
