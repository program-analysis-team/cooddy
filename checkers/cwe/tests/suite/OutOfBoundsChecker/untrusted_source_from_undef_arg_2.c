char* fgets(void*, int, void*);

void foo()
{
    int arr[10] = {};
    int idx = 0;
    char* c = fgets(&idx, sizeof(int), 0);
    arr[idx];  // error here
    arr[*c];    // error here as well
}
