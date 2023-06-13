#pragma once

#include <stddef.h>
#include <string>
#include <stdexcept>

#include "DataTypeInator.hh"
#include "Type/ModifierType.hh"


/**
 */
class PointerDataType : public ModifierType {

    public:

    /**
     Constructor for ArrayDataType.
     @param DataTypeInator The Type resolver machine
     @param typeSpecifierName Name of the type on with this type is based.
     */
    PointerDataType( DataTypeInator * dataTypeInator,
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
     bool validate();

    /**
     @return The size (in bytes) of an instance of the PointerDataType.
     */
    size_t getSize() const ;

    /**
     */
    TypeClass::e getTypeClass() const {
        return TypeClass::POINTER;
    }

    /**
     */
    bool containsPointers() const;

    /**
     */
    DataType * clone () const;

    /**
     Create zero or more instances of this DataType.
     */
    void* createInstance(unsigned int num) const ;

    /**
     */
    void deleteInstance(void* address) const;

    /**
     */
    void clearValue(void * address) const;

    /**
     */
    void assignValue(void * address, Value * value) const;

    /**
     @param s The stream to print to.
     @param base_addr Address of the (entire) variable.
     */
    void printValue(std::ostream &s, void * address ) const;


    Value * getValue(void *address) const;


    /**
     */
    std::string getTypeSpecName() const;

    /**
     */
    std::string makeDeclaration(std::string declarator) const;
    
    // virtual bool lookupVariableNameByOffset(VariableNameStack& nameStack, unsigned int offset, const DataType * expectedType) const;


    private:
    // bool initPointerDataType( DataTypeInator * dataTypeInator, std::string typeSpecName);
    PointerDataType(){}

    bool             is_valid;
    std::string      typeSpecName;
    // const DataType * subType;

    // TypeDictionary * typeDictionary;
    DataTypeInator * dataTypeInator;
};
