class Pointer {
private:
    int rc;

public:
    void destroy() {
        this->rc--;
        delete this;
    }
};
