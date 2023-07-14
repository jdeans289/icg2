#pragma once

#include <stddef.h>
#include <string>
#include <stdexcept>

#include "DataTypeInator.hpp"
#include "Type/ModifierType.hpp"


/**
 */
class PointerDataType : public ModifierType {

    public:

    /**
     Constructor for ArrayDataType.
     @param DataTypeInator The Type resolver machine
     @param typeSpecifierName Name of the type on with this type is based.
     */
    PointerDataType( const DataTypeInator * dataTypeInator,
                     std::string typeSpecifierName ) ;

    /* ==================================================================== */
    /*                         RULE OF THREE INTERFACE                      */
    /* ==================================================================== */

    /**
     Copy Constructor for PointerDataType.
     */
    PointerDataType ( const PointerDataType & original );

    /**
     Destructor for PointerDataType.
     */
    ~PointerDataType ();

    /**
     Assignment operator for PointerDataType.
     @param rhs right-hand-side.
     */
    PointerDataType& operator=( const PointerDataType & rhs );

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
        return TypeClass::POINTER;
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


    /**
     */
    std::string getTypeSpecName() const override;

    /**
     */
    std::string makeDeclaration(std::string declarator) const override;
    
    bool accept (DataTypeVisitor* visitor) const override;


    private:
    PointerDataType(){}

    bool             is_valid;
    std::string      typeSpecName;

    const DataTypeInator * dataTypeInator;
};
