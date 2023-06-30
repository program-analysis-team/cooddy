
int allocStructureFromPubPool(int size, int** ptr);

void foo(int** ptr)
{
    int res = allocStructureFromPubPool(1, ptr);
    if (res == 0 || *ptr == 0) {
        return;
    }
}
