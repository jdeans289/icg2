#pragma once

#include "ModifierType.hh"
#include "DataTypeInator.hh"
#include "VarAccessInfo.hh"

#include <stddef.h>
#include <string>
#include <stdexcept>

// #define MAX_CONSTRAINED_DIMS 8

// class LexicalAnalyzer;

/**
 */
class ArrayDataType : public ModifierType {

    public:
    /**
     Constructor for ArrayDataType.
     @param typeSpecifierName Name of the type on with this type is based.
     @param n_elems Number of elements.
     */
    ArrayDataType( DataTypeInator* inator,
                   std::string typeSpecifierName,
                   unsigned int n_elems )  ;

    /**
     Create an ArrayDataType with a different elementCount, but otherwise
     identical to the original.
     */
    ArrayDataType ( const ArrayDataType & original, unsigned int newSize );

    /* ==================================================================== */
    /*                         RULE OF THREE INTERFACE                      */
    /* ==================================================================== */

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
    ArrayDataType& operator=( const ArrayDataType & rhs );

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */

    /**
     */
    bool containsPointers() const;

    /**
     */
    DataType * clone () const;

    /**
     */
     bool validate() override;

    /**
     @return The size (in bytes) of an instance of the ArrayDataType.
     */
    size_t getSize() const override ;

    TypeClass::e getTypeClass() const override {
        return TypeClass::ARRAY;
    }

    /**
     @return The number of arrayed Type-specifier elements of the DataType.
    */
    unsigned int getTotalElementCount() const;

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

    /**
     @param s The stream to print to.
     @param base_addr Address of the (entire) variable.
     */
    void printValue(std::ostream &s, void * address ) const override;

    /**
     */
    std::string getTypeSpecName() const override;

    /**
     */
    std::string makeDeclaration(std::string declarator) const override;

    void accept(DataTypeVisitor * visitor) const override;

    virtual bool lookupVariableNameByOffset(VariableNameStack& nameStack, unsigned int offset, const DataType * expectedType) const;


    /* ==================================================================== */
    /*                       CLASS SPECIFIC INTERFACE                       */
    /* ==================================================================== */

    /**
     */
    // bool getElementInfo( LexicalAnalyzer* lexer, void* baseAddress, VarAccessInfo& varAccessInfo);

    /**
     @return Returns number of elements in this level of the array
     */
    unsigned int getElementCount() const;

    // /**
    //  */
    // const DataType * getSubType() const;


    private:
    bool initArrayDataType( DataTypeInator* dataTypeInator, std::string typeSpecName, unsigned int n_elems );
    ArrayDataType(){}

    size_t typeSize;
    unsigned int elementCount;
    bool is_valid;
    std::string typeSpecName;

    DataTypeInator* dataTypeInator;

};

