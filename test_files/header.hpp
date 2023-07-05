/*
PURPOSE:
    ( Basic header to test parsing )
PROGRAMMERS:
    ()
*/
#include "included_header.hpp"
#include <memory>

class MyClass
{
public:
    // virtual void method() const = 0;
    static int static_method();

    static const int my_static_field;  /* m is a comment */

    int my_int; 
    Foo my_nested_class;

private:

};