#pragma once

#include "Type/BaseType.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/FloatingPointValue.hpp"

#include <stdlib.h>
#include <iostream>

/**
 */
class PrimitiveDataType : public BaseType {

public:

    /**
     Constructor for PrimitiveDataType.
     */
    PrimitiveDataType() {}
    ~PrimitiveDataType() {}

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                           */
    /* ==================================================================== */

    // Most of the DataTypeInterface is abstract here, will be defined by SpecifiedPrimitiveType

    TypeClass::e getTypeClass() const {
        return TypeClass::PRIMITIVE;
    }


    bool accept (DataTypeVisitor* visitor) const override {
        return visitor->visitPrimitiveDataType(this);
    }


    /* ==================================================================== */
    /*                       CLASS SPECIFIC INTERFACE                       */
    /* ==================================================================== */

    /**
     Return true if this type represents is float or double. Otherwise return false.
     */
    virtual bool isFloatingPoint() const = 0;

    /**
     Return true if this type represents a char, short, int, long, or long long. Otherwise return false.
     */
    virtual bool isSigned() const = 0;

    /**
     Return true if this type represents void. Return false otherwise.
     */
    virtual bool isVoid() const = 0;

    /**
     * Print an ascii representation of the value at the given address to the stream.
     */
    virtual void printValue(std::ostream &s, void *address ) const = 0;


};