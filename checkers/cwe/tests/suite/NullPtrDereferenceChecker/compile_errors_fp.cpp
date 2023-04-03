
struct S1;

struct S {
    int field;
};

void boo1(void* p)
{
    S* s = 0;
    s->field = 0;  // NPD here
}

void boo(void* p)
{
    S* s = 0;
    s = (S*)((char*)p + sizeof(S1));
    s->field = 0;  // no error here due to compilation errors
}

void boo2(void* p)
{
    S* s = 0;
    s->field = 0;  // NPD here
}
