#pragma once

#include <map>
#include <string>

#include "Type/DataType.hpp"
#include "Type/StructMember.hpp"

#include "DataTypeInator.hpp"


/**
 A CompositeDataType represents user-defined types - structs and classes.
 It is 'composed' of ordered lists of component types.
 */
class CompositeDataType : public DataType {

public:

    typedef std::map<std::string, StructMember> MemberMap;
    typedef std::vector<StructMember*> SortedMemberList;

    /**
     Constructor for CompositeDataType.
     @param sizeof_struct Size, in bytes, of the struct, class or union that this class represents.
     */
    CompositeDataType( std::string name,
                       size_t sizeof_struct,
                       void *(*allocator)(int),
                       void (*deAllocator)(void*) );

    /* ================================================================================= */
    /*                             Rule of 3 - NO COPYING                                */
    /* ================================================================================= */

    CompositeDataType ( const CompositeDataType & original ) = delete;
    ~CompositeDataType ();

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */


    /**
     */
    bool validate(DataTypeInator* dataTypeInator) override;

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
     * @brief Get the map of struct members in this object
     * 
     * @return MemberMap& reference to the MemberMap object
     */
    virtual const MemberMap& getMemberMap () const = 0; 

    /**
     * @brief Get the Sorted Member List object
     * 
     * @return const SortedMemberList 
     */
    const SortedMemberList& getSortedMemberList () const;


    /**
     * @brief Look up whether this struct has a member with the given name
     * 
     * @param name name of member
     * @return true struct has member with that name
     * @return false otherwise
     */
    bool hasMemberNamed(std::string name);


protected:
    // A non-const member map accessor, should only be used for validation
    virtual MemberMap& getMemberMap () = 0; 

private:
    SortedMemberList sorted_members;

    bool is_valid;

    std::string name;
    size_t structSize;

    void* (*allocator)(int);
    void (*deAllocator)(void*);
};
