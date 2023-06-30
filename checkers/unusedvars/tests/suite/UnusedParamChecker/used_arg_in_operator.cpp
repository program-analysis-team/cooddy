class A {
public:
    A(int val) : value(val){};

    A& operator+=(const A& virtualOffset)
    {
        for (auto offset = virtualOffset.value; offset != 0; offset++) {
            return *this;
        }
        return *this;
    }

private:
    int value;
};
