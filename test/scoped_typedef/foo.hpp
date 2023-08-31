#include <string>
#include <vector>

typedef std::string MyString;
typedef std::vector<MyString> StrVec;

namespace Jackie {
    typedef std::string JString;
}

class A {
public:
    MyString x;

    typedef MyString MyOtherString;

    MyOtherString s;
    Jackie::JString jstring;
};

namespace Jackie {
    class B {
    public:
        A::MyOtherString s;
        JString jstring;
        StrVec v;
    };
}

