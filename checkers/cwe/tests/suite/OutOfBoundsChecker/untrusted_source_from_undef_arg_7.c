typedef struct {
    unsigned int index;
} FooStruct;

int g_FooArray[8];

int SomeFunc(FooStruct *par);

int GetObject(unsigned int id) {
    return g_FooArray[id];
}

int Foo() {
    FooStruct res;
    SomeFunc(&res);
    return GetObject(res.index);
}
