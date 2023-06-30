//  It is minimized example of null pointer dereference which was found in the SQLite version 3.31.3

#define deref(x) (*x)

struct myStruct {
    myStruct *pStruct;
    union {
        int *pInt;
        char *pChar;
    } a;
};

int main()
{
    myStruct *s;

    s->pStruct = new myStruct;
    s->pStruct->a.pInt = 0;

    deref(s->pStruct->a.pInt) = 10; //  pInt is equal to null pointer here and it is dereferenced
}
