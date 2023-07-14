#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include "Type/DataType.hpp"
#include "Type/StructMember.hpp"
#include "Type/BitfieldStructMember.hpp"

#include "Value/Value.hpp"

#include "DataTypeInator.hpp"


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

    /* ================================================================================= */
    /*                         RULE OF THREE (and a half) INTERFACE                      */
    /* ================================================================================= */

    CompositeDataType ( const CompositeDataType & original );
    ~CompositeDataType ();
    CompositeDataType & operator=( CompositeDataType rhs ) ;

    friend void swap (CompositeDataType& a, CompositeDataType& b) ;

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */

    /**
     */
    DataType * clone() const override;

    /**
     */
    bool validate() override;

    bool isValid() const override;

    /**
     @return The size (in bytes) of an instance of the CompositeDataType.
     */
    size_t getSize() const override;

    /**
     */
    TypeClass::e getTypeClass() const override {
        return TypeClass::COMPOSITE;
    };

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
     Assign a value to the variable at the given address.
     @param address Base-address of the (possibly arrayed) variable.
     @param value The Value to be assigned to the element.
     */
    void assignValue(void * address, Value * value) const override;

    Value * getValue(void * address) const override;

    /**
     */
    std::string toString() const override;

    bool accept (DataTypeVisitor* visitor) const override;

    /**
     */
    std::string getTypeSpecName() const override;

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
    std::string name;
    size_t structSize; /** Sizeof the struct/or class represented by the CompositeDataType. */
    void* (*allocator)(int);
    void (*deAllocator)(void*);
    DataTypeInator* dataTypeInator;
};
