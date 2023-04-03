typedef struct Object {
    int value;
} ObjectStruct;

int g_VAR;
ObjectStruct *g_ObjStruct();

ObjectStruct *GetObject() {
    if (g_VAR < 0) {
        return 0;
    }
    return g_ObjStruct();
}

int Foo(int id) {
    ObjectStruct *object1 = GetObject(id);
    if (object1 == 0) {  // Pointer is checked here for null
        return 1;
    }
    ObjectStruct *object = GetObject(id);
    if (object->value == 0) {
        return 1;
    }
    return 0;
}
