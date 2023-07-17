/*
PURPOSE:
    ( Work with namespaces )
PROGRAMMERS:
    ()
*/
#include "foo.hpp"

class Bar {
public:
    jackie::Foo j_foo;
    Foo foo;

    // Foo::Bar foobar;
};

namespace jackie {
    class j_Bar {
    public:
        Foo j_foo;
        ::Foo foo;
    };
}