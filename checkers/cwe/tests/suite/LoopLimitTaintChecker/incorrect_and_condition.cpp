struct S {int result; };

void some_undef_func(S *d);

void foo()
{
    S a;
    do {
        some_undef_func(&a);
    } while ((0 == a.result) && (0 == a.result));
}
