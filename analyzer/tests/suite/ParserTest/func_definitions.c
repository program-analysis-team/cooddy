
void first_func(param)
    int param;
{
}

void func_with_multiline_params_without_definition(
    int*,
    int*);

void func_with_multiline_params_with_definition(
    int*,
    int*) {}

void some_func()
{
    int a = 0;
#ifdef SOME_DEFINE
    for (int i = 0; i < 10; ++i) {
#elseif SOME_DEFINE_Z
    for (int i = 0; i < 20; ++i) {
#else
    for (int i = 0; i < 30; ++i) {
#endif
    }
}

void tail_func() {}
