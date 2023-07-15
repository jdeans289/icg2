#pragma once

#include "Type/ModifierType.hpp"
#include "DataTypeInator.hpp"

#include <stddef.h>
#include <string>
#include <stdexcept>


/**
 */
class ArrayDataType : public ModifierType {

    public:
    /**
     Constructor for ArrayDataType.
     @param typeSpecifierName Name of the type on with this type is based.
     @param n_elems Number of elements.
     */
    ArrayDataType( const DataTypeInator* inator,
                   std::string typeSpecifierName,
                   unsigned int n_elems )  ;

    /**
     Create an ArrayDataType with a different elementCount, but otherwise
     identical to the original.
     */
    ArrayDataType ( const ArrayDataType & original, unsigned int newSize );

    /* ================================================================================= */
    /*                         RULE OF THREE (and a half) INTERFACE                      */
    /* ================================================================================= */

    /**
     Copy Constructor for ArrayDataType.
     */
    ArrayDataType ( const ArrayDataType & original );

    /**
     Destructor for ArrayDataType.
     */
    ~ArrayDataType ();

    /**
     Assignment operator for ArrayDataType.
     @param rhs right-hand-side.
     */
    ArrayDataType& operator=( ArrayDataType rhs );

    friend void swap (ArrayDataType& a, ArrayDataType& b);

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */

    /**
     */
    DataType * clone () const;

    /**
     */
     bool validate() override;

     bool isValid() const override;


    /**
     @return The size (in bytes) of an instance of the ArrayDataType.
     */
    size_t getSize() const override ;

    /**
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

    Value * getValue(void * address) const override;

    /**
     */
    std::string getTypeSpecName() const override;

    /**
     */
    std::string makeDeclaration(std::string declarator) const override;

    bool accept(DataTypeVisitor * visitor) const override;

    // virtual bool lookupVariableNameByOffset(MutableVariableName& nameStack, unsigned int offset, const DataType * expectedType) const;


    /* ==================================================================== */
    /*                       CLASS SPECIFIC INTERFACE                       */
    /* ==================================================================== */

    /**
     */
    // bool getElementInfo( LexicalAnalyzer* lexer, void* baseAddress, VarAccessInfo& varAccessInfo);

    /**
     @return The number of arrayed Type-specifier elements of the DataType.
    */
    unsigned int getTotalElementCount() const;

    /**
     @return Returns number of elements in this level of the array
     */
    unsigned int getElementCount() const;

    // /**
    //  */
    // const DataType * getSubType() const;


    private:
    bool initArrayDataType( const DataTypeInator* dataTypeInator, std::string typeSpecName, unsigned int n_elems );
    ArrayDataType(){}

    size_t typeSize;
    unsigned int elementCount;
    bool is_valid;
    std::string typeSpecName;

    const DataTypeInator* dataTypeInator;

};

