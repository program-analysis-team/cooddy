

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

int foo(mymap<int>& map1, mymap<int>& map2, int v)
{
    auto it = map1.find();
    if (map2.find() == map2.end() && v == 10 || it == map1.end()) {
        return 0;
    }
    return *it;  // no NPD here
}
