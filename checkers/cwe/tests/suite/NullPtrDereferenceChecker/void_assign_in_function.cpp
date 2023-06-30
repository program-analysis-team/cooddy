void func(void* fo111, int fo222) {
    *((int*)fo111 + 5) = fo222;
}

int main() {
    int* arr[10];
    func(arr, 0);
    return *arr[5]; //should be detected here
}
