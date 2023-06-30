void func(int* fo111[10], int* fo222[5]) {
    fo111[1] = fo222[1];
}

int main() {
    int* a[10];
    int* b[5];
    b[1] = 0;
    func(a, b);
    return *a[1];
}
