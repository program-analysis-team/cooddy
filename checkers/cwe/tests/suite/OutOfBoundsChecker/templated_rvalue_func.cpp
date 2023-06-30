#include <utility>
#define SZ 42

void foo() {
    int arr[SZ];
    for(int i=0; i<SZ; i++) {
        auto p = std::make_pair(arr[i], i);
        arr[i];
    }
}
