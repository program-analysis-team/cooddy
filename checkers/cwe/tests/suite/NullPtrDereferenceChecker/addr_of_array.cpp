struct sMarshalType {
    int* m_elements;
};

void foo(sMarshalType* type)
{
    if (type == nullptr) {
        return;
    }
    auto* elems = &type->m_elements[0];
    if (elems == nullptr) {
        return;
    }
    *elems = 42;
}
