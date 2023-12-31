#ifndef VALUE_H
#define VALUE_H

#include <string>

/**
 @brief Value is an abstract base-class that represents a value on the right-hand-side of an assignment.
*/
class Value {

   public:

    /**
     Destroy the Value.
     */
    virtual ~Value() {};

    /**
     Print a representation of this Value to the given stream.
     */
    virtual void print(std::ostream &s) const = 0;

    /**
     Get a string representation of this Value.
     */
    virtual std::string toString() const = 0;

    enum ValueType {
        STRING,
        INTEGER,
        FLOATING_POINT,
        POINTER
    };

    virtual ValueType getValueType() const = 0; 

};
#endif
