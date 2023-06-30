typedef struct Bar_S {
    int len;
} BAR;

typedef struct Foo_S {
    BAR entrys[10];
} FOO;

void Some_func(FOO **curSrc, int *curIndex, int *curOffset);

int main() {
    FOO *curSrc = 0;
    int curIndex = 0;
    int curOffset = 0;
    Some_func(&curSrc, &curIndex, &curOffset);
    int ret = curSrc->entrys[curIndex].len - curOffset;
    return ret;
}
