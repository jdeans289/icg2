#pragma once

#include <string>
#include <vector>

#include "Value.hh"
#include "DataTypeInator.hh"

// class DataTypeInator;
class DataType;

namespace MemberClass {
    enum e {
        NORMAL   = 0,
        STATIC   = 1,
        BITFIELD = 2
    };
};

/**
 StructMember represents a data-member of a CompositeDeclaration.
 */
class StructMember {

public:
    /**
     Constructor for StructMember.
     @param name The name of the member.
     @param dataTypeInator Pointer to the DataTypeInator used for resolving types.
     @param typeSpecName string representing the type
     */
    StructMember(std::string name, DataTypeInator* dataTypeInator, std::string typeSpecName);

    /**
    Constructor for StructMember.
    @warning This should ONLY be called by a constructor of a derived class.
    */
    StructMember (const StructMember & original);

    /**
    Clone.
    */
    virtual StructMember * clone () const = 0;

    /**
    Destructor.
    */
    virtual ~StructMember () { /* Nothing to do */ }

    /**
     Get the name of the member.
     @return std::string containing the name of the member.
     */
    std::string getName() const;

    /** 
     * This should maybe die? idk -jackie
     */
    bool containsPointers() const ;


    /**
      To check for circular reference only.
     */
    const DataType * getDataType();

    /**
    */
    virtual MemberClass::e getMemberClass() const = 0;

    /**
     */
    virtual bool validate();

    /**
     @return does the DataType or any member of the DataType represent a pointer?
     Do we actually want this? -jackie
     */
    virtual bool containsPointer() const { return false; }

    /**
     */
    virtual void clearValue(void *struct_address) const = 0;

    /**
     Assign a value to the data-member, described by this StructMember,
     that is within in an instance of the composite-type described by
     the parent CompositeDeclaration.

     @param struct_address Address of an instance of the composite type.
     */
    virtual void assignValue(void *struct_address,  Value *v) const = 0;

    /**
     @param s
     @param struct_address
     */
    virtual void printValue(std::ostream &s, void *struct_address) const = 0;


    // virtual void checkpointValue(std::ostream &s, std::string var_name, void *address) const = 0;


    /**
    Get a string representation of this StructMember.
     */
    virtual std::string toString() const = 0;

protected:

    // This is meant for bitfield members only
    // this is bad design and you know it @me
    StructMember(std::string name);

    std::string typeSpecName;
    const DataType * subType;
    DataTypeInator* dataTypeInator;
    bool isValid;

private:
    std::string name;

};