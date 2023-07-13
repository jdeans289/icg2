#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include "Type/DataType.hpp"
#include "Type/StructMember.hpp"
#include "Type/BitfieldStructMember.hpp"

#include "Value/Value.hpp"

#include "DataTypeInator.hpp"


class LexicalAnalyzer;

/**
 A CompositeDataType represents user-defined types, such as structs, unions, or classes.
 It is 'composed' of ordered lists of component types.
 */
class CompositeDataType : public BaseType {

public:

    /**
     Constructor for CompositeDataType.
     @param sizeof_struct Size, in bytes, of the struct, class or union that this class represents.
     */
    CompositeDataType( DataTypeInator* dataTypeInator,
                       std::string name,
                       size_t sizeof_struct,
                       void *(*allocator)(int),
                       void (*deAllocator)(void*) );

    /* ==================================================================== */
    /*                         RULE OF THREE INTERFACE                      */
    /* ==================================================================== */

    /**
     Copy Constructor.
     @param original The instance of CompositeDataType that is to be copied.
     */
    CompositeDataType ( const CompositeDataType & original );

    /**
     Destructor for CompositeDataType.
      */
    ~CompositeDataType ();

    /**
     Assignment operator for CompositeDataType.
     @param rhs right-hand-side.
    */
    CompositeDataType & operator=( const CompositeDataType & rhs );

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */

    /**
     */
    DataType * clone() const;

    /**
     */
    bool validate() ;

    /**
     @return The size (in bytes) of an instance of the CompositeDataType.
     */
    size_t getSize() const ;

    /**
     */
    TypeClass::e getTypeClass() const {
        return TypeClass::COMPOSITE;
    };

    /**
     */
    void* createInstance(unsigned int num) const;

    /**
     */
    void deleteInstance(void* address) const;

    /**
     */
    void clearValue(void * address) const;

    /**
     Assign a value to the variable at the given address.
     @param address Base-address of the (possibly arrayed) variable.
     @param value The Value to be assigned to the element.
     */
    void assignValue(void * address, Value * value) const;

    Value * getValue(void * address) const;

    /**
     Print the value at the given address, to the given stream.
     @param s The stream to print to.
     @param address Address of the (entire) variable.
     */
    void printValue(std::ostream &s, void *address) const;

    /**
     Print the value at the given address, to the given stream.
     @param s The stream to print to.
     @param var_name Name of the variable to checkpoint.
     @param address Address of the (entire) variable.
     */
    // void checkpointValue(std::ostream &s, std::string var_name, void *address) const;

    /**
     */
    std::string toString() const;

    // virtual bool lookupVariableNameByOffset(MutableVariableName& nameStack, unsigned int offset, const DataType * expectedType) const;

    bool accept (DataTypeVisitor* visitor) const override;

    /* ==================================================================== */
    /*                       CLASS SPECIFIC INTERFACE                       */
    /* ==================================================================== */

    /**
     Add a regular, non-bitfield, non-static data member to the CompositeDataType.
     @param memberName Name of the data member.
     @param offset The offset (in bytes) of the data-member from the beginning of
     the struct, union or class.
     @param typeName
     @param n_dims
     @param dims
     */
    void addRegularMember( std::string memberName,
                           int offset,
                           std::string typeName) ;

    /**
     Add a static data member to the CompositeDataType.
     @param memberName Name of the data member.
     @param memberAddress The offset (in bytes) of the data-member from the beginning of
     the struct, union or class.
     @param typeName
     @param n_dims
     @param dims
     */
    void addStaticMember(std::string memberName,
                         void * memberAddress,
                         std::string typeSpecName ) ;

    /**
     Add a bitfield data member to the CompositeDataType.
     @param member_name Name of the data member.
     @param getter Pointer to a function that returns the value of a bitfield in the addressed class/struct.
     @param getter Pointer to a function that sets the value of a bitfield in the addressed class/struct.
     */
    template <class T> void addBitFieldMember( std::string member_name,
                                               T(*getter)(void* address),
                                               void(*setter)(void* address, T value) ) {

        memberList.push_back( new BitfieldStructMember<T>(member_name, getter, setter));

    }

    /**
     */
    std::string getTypeSpecName() const;

    /**
     */
    int getMemberCount() const;

    /**
     */
    StructMember* getStructMember (const int index) const;

    /**
     */
    StructMember* getStructMember (std::string name) const;


private:
    CompositeDataType();

    bool is_valid;
    std::vector<StructMember*> memberList;
    std::vector<StructMember*>::iterator memberListIterator;
    std::string name;
    size_t structSize; /** Sizeof the struct/or class represented by the CompositeDataType. */
    void* (*allocator)(int);
    void (*deAllocator)(void*);
    DataTypeInator* dataTypeInator;
};
