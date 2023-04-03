
int g_v = 0;

struct Foo {
    int v;
};

int main(int argc, char *argv[])
{
    if (argc == 1) {
        argc = 2;
        goto err;
    }
    int v = argc;
err:
    argc = g_v;  // no error here
    struct Foo foo;  // no error here
    foo.v = 10;

    return v;    // error here
}
