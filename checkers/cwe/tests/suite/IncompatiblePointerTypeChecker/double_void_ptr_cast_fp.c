#define NULL 0

void foo(void** ptr);

typedef struct StructA {
    int a;
    int b;
} StructObj;

int main()
{
    StructObj* someStruct = NULL;
    foo((void**)&someStruct);
    return 0;
}
