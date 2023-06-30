
int g_arr[] = {1, 2, 3};

enum Values { A, B, C };

struct Boo {
    Values field;
};

void foo(Boo* p)
{
    auto f = p->field;
    g_arr[f];  // no error here
}
