int memset_s(void* buff, int buffSize, char pattern, int count);

typedef struct _Foo {
    char buff[255];
    int size;
} Foo;

static void init(Foo* p)
{
    Foo foo;
    memset_s(&foo, sizeof(Foo), 0, sizeof(Foo));
    if (p == 0) {
        return;
    }
    foo.size = p->size;
    memset_s(foo.buff, 255 + 1, 0, (foo.size + 1));
}
