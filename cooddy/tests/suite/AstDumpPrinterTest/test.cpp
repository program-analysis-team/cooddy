// all printable AST nodes
#include "vector"
#include "string"
class SomeClass;
union SomeUnion;
struct SomeStruct {
    int someField;
    int SomeMethod(int param, int param2) {
        return param + 10 + param2;
    }
};
int bar(int param, int param2);
enum SomeEnum { a, b, c };
int foo(int a, void* b, SomeEnum someEnum)
{
    auto str = "abc abc";
    SomeStruct* v;
    SomeStruct v2;
    v->SomeMethod(a, 0);
    unsigned char ccc = 5;
    int matrix[10][20];
    int init[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int aaa[] = {1, 2, 3};
    int i = 10;
    bool boolVar;

    do {
    } while (0);
    while (false) {
    }
    for (auto k : {1, 2, 3}) {
    }
    for (int j = 0; j < 10; j++) {
        a++;
        a--;
        ++a;
        &a;
        a = a;
        a |= a;
        a = a + (v->someField + v2.someField) + bar(a, 10) + v->SomeMethod(a, j);
        break;
    }
    if (someEnum == SomeEnum::b) {
        return 5;
    }
    if (a == 3) {
        return 5;
    } else if (a == 4) {
        return 10;
    } else {
        return a;
    }
    a = 1;
    a = 1 + 2;
    a = 1 + 2 + 3;
    a = 1 * 2 / 3 + 4 - 5;
    boolVar = (1 | 2) & (3 ^ 4 & 5) || true;
    matrix[20][10];
    return 0;
}
