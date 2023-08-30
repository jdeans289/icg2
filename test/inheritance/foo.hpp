#include <string>

struct A {
public:
    int a;
};

struct B : public A {
    double b;
};

struct C : public B {
    std::string c;
};