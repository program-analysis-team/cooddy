#include <iostream>

typedef struct myStruct {
    int index;
    int foo()
    {
        std::cerr << "Hi from " << index << " foo\n";
        return 0;
    }
}myStruct;

int func(myStruct* a[], int length)
{
    std::cerr << (a[0] == nullptr) << "\n";
    a[1]->foo();
    for(int i = 0; i < length; ++i)
        return a[i]->foo();
    return -1;
}

int main() {
    myStruct* arr[] = {new myStruct{1}, nullptr};
    return func(arr, 2);
};
