#pragma once

#include "Value/Value.hpp"
#include <vector>

/**
 ArrayValue is a list of Value(s) of the same type.
 */
class ArrayValue : public Value {

    public:

    /**
     Destroy this ArrayValue.
     */
    ~ArrayValue();

    /**
     Add to this ArrayValue.
     */
    void pushElem(Value* value);

    /**
     Get the number of members in this ArrayValue.
     @return the number of members in this ArrayValue.
     */
    unsigned int  getNumElems();

    /**
     Get reference to the component Value at the given index.
     */
    Value *& operator[] (const int index);

    /**
     Print this ArrayValue to a stream.
     */
    void  print(std::ostream &s);

    /**
     Get a string representation of this ArrayValue.
     */
    std::string toString();

    private:
    std::vector<Value*> elem_list;
};
