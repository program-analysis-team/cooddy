class A {};

class B : public A {};

int main() {
    B* b = new B();
    auto x = dynamic_cast<A*>(b);
}
