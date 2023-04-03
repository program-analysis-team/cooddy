#include <memory.h>
#include <stdlib.h>
#include <iostream>
struct SomeStruct {
    int idx;
    int initialize(int a){
        idx = a;
        if (a > 0)
            return 1;
        return 0;
    }
    void PrintHello(){
        std::cout << "Hello" << std::endl;
    }
};
class A {
public:
    A();
    int initialize();
    int use();
private:
    SomeStruct * pSomeStruct;
};
A::A() {
    pSomeStruct = NULL;
}
int A::initialize() {
    if (pSomeStruct != NULL) {
        // MACROS_DELETE pSomeStruct;
        delete pSomeStruct;
        pSomeStruct = NULL;
    }
    pSomeStruct = new SomeStruct;
    if (pSomeStruct == NULL){
        return 1;
    }
    int rc = pSomeStruct->initialize(0);
    if (rc != 0) {
        delete pSomeStruct;
        pSomeStruct = NULL;
        return 2;
    }
    return 0;
}
int A::use() {
    if (!pSomeStruct){
        return 1;
    }
    pSomeStruct->PrintHello();
    return 0;
}
void Foo()
{
    A *pA;
    pA = new A;
    int rc = pA->initialize();
    if(rc) {
        delete pA;
        return;
    }
    pA->use();
}
