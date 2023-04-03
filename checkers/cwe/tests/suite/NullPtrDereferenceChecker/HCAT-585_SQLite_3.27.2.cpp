//  It is minimized example of null pointer dereference which was found in the SQLite version 3.27.2

#define MACRO(pInt) (pInt << 37)

struct Struct;

struct MyStruct {
    Struct *pStruct;
};

struct Struct {
    int pInt;
};

int main()
{
    MyStruct *s;
    s = new MyStruct;
    s->pStruct = 0;

    MACRO(s->pStruct->pInt);    //  's->pStruct' is null here, using 's->pStruct->pInt' causes null pointer dereference
}
