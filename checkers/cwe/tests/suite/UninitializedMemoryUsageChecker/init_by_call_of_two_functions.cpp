typedef struct {
    bool trav;
    bool isCompleted;
} ClientRecIter;

static void InitIterInner(ClientRecIter *iter)
{
    iter->trav = true;
    iter->isCompleted = false;
}

static void InitIter(ClientRecIter *iter)
{
    InitIterInner(iter);
}

int main()
{
    int ret;
    ClientRecIter iter;
    InitIter(&iter);

    while (!iter.isCompleted) {
        ret = 0;
        iter.isCompleted = true;
    }
    return ret;
}
