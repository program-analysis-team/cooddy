#include <iostream>

typedef struct myStruct {
    int index;
    int foo()
    {
        std::cerr << "Hi from " << index << " foo\n";
        return 0;
    }
}myStruct;

int main() {
    myStruct* arr[] = {nullptr, new myStruct{1}};
    arr[0]->foo();
};
