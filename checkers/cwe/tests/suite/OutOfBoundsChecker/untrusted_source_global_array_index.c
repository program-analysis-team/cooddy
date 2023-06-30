#define MAX 16

int g_Arr[MAX];

unsigned int SomeFunc();

int* GetObject(int id)
{
    return &g_Arr[id];
}

void Foo()
{
    unsigned int threadId = SomeFunc();
    int* count = GetObject(threadId);
}
