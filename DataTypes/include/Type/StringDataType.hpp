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

    /* ================================================================================= */
    /*                         RULE OF THREE (and a half) INTERFACE                      */
    /* ================================================================================= */

    StringDataType ( const StringDataType & original ) = default;
    ~StringDataType () = default;
    StringDataType& operator=( const StringDataType & rhs ) = default;

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
    DataType * clone () const override;

    /**
     Create zero or more instances of this DataType.
     */
    void* createInstance(unsigned int num) const override;

    /**
     */
    void deleteInstance(void* address) const override;

    std::string getTypeSpecName () const override;

    
    bool accept (DataTypeVisitor* visitor) const override;


    private:
};
