void func(int* fo111[10], int* fo222[5]) {
    fo111[1] = fo222[0];
}

int main() {
    int* a[10];
    int* b[5];
    b[0] = 0;
    func(a, b);
    return *a[1];
}
