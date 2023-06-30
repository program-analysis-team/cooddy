
struct Boo {
    int idx;
};

int freeStructure(Boo*);

void foo(Boo* boo)
{
    int arr[10] = {0};
    arr[boo->idx]; // no error here
    freeStructure(boo);
}
