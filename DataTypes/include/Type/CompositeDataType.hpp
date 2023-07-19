#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include "Type/DataType.hpp"
#include "Type/StaticStructMember.hpp"
#include "Type/NormalStructMember.hpp"
// #include "Type/BitfieldStructMember.hpp"

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
    void* createInstance(unsigned int num) const override;

    /**
     */
    void deleteInstance(void* address) const override;

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

        // memberList.push_back( new BitfieldStructMember<T>(member_name, getter, setter));

    }

    typedef std::vector<NormalStructMember *> NormalMemberList;
    typedef std::vector<StaticStructMember *> StaticMemberList;

    /**
     * @brief Get the number of normal members in this composite type
     * 
     * @return int normal member count
     */
    int getNormalMemberCount() const;

    /**
     * @brief Get the number of static members in this composite type
     * 
     * @return int static member count
     */
    int getStaticMemberCount() const;

    inline NormalMemberList::iterator getNormalMemberListBegin() { return normalMemberList.begin(); }
    inline NormalMemberList::iterator getNormalMemberListEnd() { return normalMemberList.end(); }

    inline StaticMemberList::iterator getStaticMemberListBegin() { return staticMemberList.begin(); }
    inline StaticMemberList::iterator getStaticMemberListEnd() { return staticMemberList.end(); }

    inline NormalMemberList::const_iterator getNormalMemberListBegin() const { return normalMemberList.begin(); }
    inline NormalMemberList::const_iterator getNormalMemberListEnd() const { return normalMemberList.end(); }

    inline StaticMemberList::const_iterator getStaticMemberListBegin() const { return staticMemberList.begin(); }
    inline StaticMemberList::const_iterator getStaticMemberListEnd() const { return staticMemberList.end(); }

private:
    CompositeDataType();

    // This is only intended to ensure we don't add two of the same members
    bool hasMemberNamed(std::string name);

    bool is_valid;
    NormalMemberList normalMemberList;
    StaticMemberList staticMemberList;
    // std::vector<BitfieldStructMember> BitfieldStructMember;

    std::string name;
    size_t structSize;

    void* (*allocator)(int);
    void (*deAllocator)(void*);

    DataTypeInator* dataTypeInator;
};
