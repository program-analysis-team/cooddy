#include <memory>
#include <string>

std::shared_ptr<std::string> global;

struct StringPtr {
    void Set(const ::std::string* default_value, const ::std::string* value)
    {
        if (ptr_ == default_value) {
            ptr_ = new std::string(*value); //FP is reported here
        } else {
            *ptr_ = *value;
        }
    }

    void Destroy()
    {
        delete ptr_;
        ptr_ = nullptr;
    }

    std::string* ptr_;
};

struct share {
    ~share()
    {
        if (name.ptr_ != nullptr)
            name.Destroy();
    }
    void set_name(const ::std::string& value)
    {
        name.Set(global.get(), &value);
    }

    StringPtr name;
};

struct B {
    share* get_share()
    {
        if (a == nullptr) {
            a = new share();
        }
        return a;
    }

    ~B()
    {
        delete a;
    }
    share* a;
};

void test(B& b)
{
    std::string val;
    auto share = b.get_share();
    share->set_name(val);
}
