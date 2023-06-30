typedef struct Object {
    int value;
} ObjectStruct;

int g_maxDpNum = 0;
ObjectStruct g_ObjStruct;

ObjectStruct *GetObject(int id) {
    if (g_maxDpNum < id) {
        return 0;
    }
    return &(g_ObjStruct);
}

int Foo(int id) {
    if (GetObject(id) == 0) {
        return 1;
    }
    ObjectStruct *object = GetObject(id);
    if (object->value == 0) {
        return 1;
    }
    return 0;
}
