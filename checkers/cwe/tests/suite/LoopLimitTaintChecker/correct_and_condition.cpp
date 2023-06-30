struct S {int result; } dbCtx;

void some_undef_func(S *d);

void foo()
{
    int retryTimes = 3;
    do {
        some_undef_func(&dbCtx);
    } while ((0 == dbCtx.result) && (0 != (--retryTimes)));
}
