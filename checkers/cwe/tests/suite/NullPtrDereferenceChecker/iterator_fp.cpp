

template <class T>
struct mymap {
    struct iterator {
        iterator(T* _p) : p(_p) {}

        T& operator*()
        {
            return *p;
        }

        bool operator==(const iterator& it)
        {
            return p == it.p;
        }

        T* p;
    };

    iterator end()
    {
        return iterator(nullptr);
    }
    iterator find()
    {
        return end();
    }
};

int foo(mymap<int>& map)
{
    auto it = map.find();
    if (it == map.end()) {
        return 0;
    }
    return *it;  // no NPD here
}
