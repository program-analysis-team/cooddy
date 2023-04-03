
struct SortedChainNode {
    int key;
    int elem;
    SortedChainNode* next;
};

template <class K, class E>
struct CSortedChainImpl {
    int Search(const K& elemKey, E*& elem) const;

    SortedChainNode* head;
};

template <class K, class E>
int CSortedChainImpl<K, E>::Search(const K& elemKey, E*& elem) const
{
    SortedChainNode* cur = head;
    if (cur->key == elemKey) {
        elem = &(cur->elem);
        return 0;
    }
    return 1;
}

template <class K, class E>
struct HashImpl {
    int Search(const K& elemKey, E*& elem) const;

    CSortedChainImpl<K, E>** chain;
};

template <class K, class E>
int HashImpl<K, E>::Search(const K& elemKey, E*& elem) const
{
    if (chain == 0) {
        return 1;
    }
    CSortedChainImpl<K, E>* p = chain[elemKey];
    if (p == 0) {
        return 1;
    }
    return p->Search(elemKey, elem);
}

int foo(HashImpl<int, int>& s, int key)
{
    int* elem = 0;
    auto ret = s.Search(key, elem);
    if (ret == 0)
        return *elem;  // no error here
    return 0;
}
