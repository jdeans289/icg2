#include "ArrayValue.hh"
#include <sstream>

// DESTRUCTOR
ArrayValue::~ArrayValue() {
    int n = elem_list.size();
    for (int ii= 0 ; ii < n; ii++) {
        delete( elem_list[ii] );
    }
}

// MEMBER FUNCTION
void ArrayValue::pushElem(Value* value) {
    elem_list.push_back(value);
}

// MEMBER FUNCTION
int ArrayValue::getNumElems() {
    return ( (int)elem_list.size() );
}

// MEMBER FUNCTION
Value*&  ArrayValue::operator[] (const int index) {
    if (index >= elem_list.size())
        throw std::logic_error("Index out of range.");

    return ( elem_list[index] );
}

// MEMBER FUNCTION
void  ArrayValue::print(std::ostream &s) {
   int n = elem_list.size();
    s << "{";
    for (int ii= 0 ; ii < n; ii++) {
        if (ii != 0) {
            s << ",";
        }
        elem_list[ii]->print(s);
    }
    s << "}";
}

// MEMBER FUNCTION
std::string  ArrayValue::toString() {
   std::ostringstream oss;

   int n = elem_list.size();
    oss << "{";
    for (int ii= 0 ; ii < n; ii++) {
        if (ii != 0) {
            oss << ",";
        }
        oss << elem_list[ii]->toString();
    }
    oss << "}";
    return oss.str();
}

