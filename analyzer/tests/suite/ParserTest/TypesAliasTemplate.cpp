template <typename T, int Size>
struct MyArray {
    typedef T ItemTypeTypedef;

    using ItemTypeAlias = T;

    T myArray[Size];
};
