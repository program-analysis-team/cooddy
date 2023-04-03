
typedef struct _Ctx
{
    void * ctx;
} Ctx;

int dplwt_task_create(int, int, void*);

int get_id();
int some_func();

int func2(Ctx* ctx)
{
    Ctx* p = ctx->ctx;
    int ret = dplwt_task_create(0, 0, p);
    if (ret != 0) {
        return ret;
    }
    return 0;
}

void* alloc(int size)
{
    if (size > 10) {
        return (char*)malloc(size) + 10;
    }
    return (char*)malloc(size) + 15;
}

int func1(void* ctx, int x)
{
    int ret;
    Ctx* c = (Ctx*)alloc(sizeof(Ctx));
    if (c == 0)
        return 1;
    c->ctx = ctx;
    ret = func2(c);
    if (ret != 0) {
        return 1;
    }
    return 0;
}

int func3(void* ctx, int x)
{
    int ret = some_func();
    if (ret != 0) {
        return ret;
    }
    if (x == get_id()) {
        ret = func2(ctx);
        return ret;
    }
    return 1;
}

void reset_ctx(Ctx* a)
{
    Ctx* c = a->ctx;
    c->ctx = 0;
}

void* func4(int x)
{
    Ctx a;

    Ctx c;
    c.ctx = &a;
    int ret = func1(&c, x);
    if (ret != 0) {
        reset_ctx(&c);
    }
    return a.ctx; // no error here
}
