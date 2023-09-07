#include <string>
#include <vector>

class Bar {
public:
    std::vector<std::string> c; // this is a comment
    unsigned long d;
};

class Foo {
public:
    int a[5][4];
    Bar b;
};