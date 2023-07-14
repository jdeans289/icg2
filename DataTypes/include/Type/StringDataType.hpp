#pragma once

#include <stddef.h>
#include <string>
#include <stdexcept>

#include "DataTypeInator.hpp"
#include "Type/BaseType.hpp"


/**
 */
class StringDataType : public BaseType {

    public:

    /**
     Constructor for ArrayDataType.
     @param DataTypeInator The Type resolver machine
     */
    StringDataType( ) ;

    /* ==================================================================== */
    /*                         RULE OF THREE INTERFACE                      */
    /* ==================================================================== */

    /**
     Copy Constructor for PointerDataType.
     */
    StringDataType ( const StringDataType & original );

    /**
     Destructor for PointerDataType.
     */
    ~StringDataType ();

    /**
     Assignment operator for PointerDataType.
     @param rhs right-hand-side.
     */
    StringDataType& operator=( const StringDataType & rhs );

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */

    /**
     */
     bool validate() override;

    bool isValid() const override;


    /**
     @return The size (in bytes) of an instance of the PointerDataType.
     */
    size_t getSize() const override;

    /**
     */
    TypeClass::e getTypeClass() const override {
        return TypeClass::PRIMITIVE;
    }

    /**
     */
    DataType * clone () const override;

    /**
     Create zero or more instances of this DataType.
     */
    void* createInstance(unsigned int num) const override;

    /**
     */
    void deleteInstance(void* address) const override;

    /**
     */
    void clearValue(void * address) const override;

    /**
     */
    void assignValue(void * address, Value * value) const override;

    Value * getValue(void *address) const override;

    std::string getTypeSpecName () const override;

    
    bool accept (DataTypeVisitor* visitor) const override;


    private:
};
