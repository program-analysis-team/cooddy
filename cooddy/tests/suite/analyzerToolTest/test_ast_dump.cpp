/**
 * Some struct
 */
struct B {int c;};

class A {
    int main(B& v)
    {
        int* p = 0;
        return *p + v.c;
    }

    int boo(int a) {
        switch(a) {
            case 1:
                return 2;
            case 2:
                return 3;
            default:
                return 4;
        }
    }
};
