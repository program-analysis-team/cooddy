#include <vector>

template <class T>
class store {
public:
    store()
    {
        rep_.resize(1);
        auto& ptr_ = rep_[0];
        ptr_ = new int(1); //FP reported here only for template class
    }

    ~store()
    {
        for (auto& v : rep_) {
            delete v;
        }
    }

    std::vector<T*> rep_;

};



int main()
{
   store<int> test;
}
