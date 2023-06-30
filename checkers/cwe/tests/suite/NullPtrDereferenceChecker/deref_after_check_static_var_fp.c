typedef struct Object {
    int value;
} ObjectStruct;

ObjectStruct *GetObject(int id)
{
    if (id < 0) {
        return 0;  // Null pointer source
    }
    static ObjectStruct objectStruct = {5};
    return &(objectStruct);
}

int GetObjectValue(int id)
{
    ObjectStruct *object = GetObject(id);  // Null pointer returned as the result
    if (object->value == 0) {              // Null dereference is reported here
        return 1;
    }
    return 0;
}

int Foo(int id)
{
    ObjectStruct *object = GetObject(id);
    if (object == (void *)0) {  // Pointer is checked here for null
        return 1;
    }
    return GetObjectValue(id);
}
