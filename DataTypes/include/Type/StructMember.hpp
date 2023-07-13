#pragma once

#include <string>
#include <vector>

#include "Value/Value.hpp"

#include "DataTypeInator.hpp"

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

    StructMember (std::string name);


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


    // /**
    //   To check for circular reference only.
    //  */
    // virtual const DataType * getDataType() const = 0;

    /**
    */
    virtual MemberClass::e getMemberClass() const = 0;

    /**
     */
    virtual bool validate() = 0;

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
     Get the value to the data-member, described by this StructMember,
     that is within in an instance of the composite-type described by
     the parent CompositeDeclaration.

     @param struct_address Address of an instance of the composite type.
     */
    virtual Value * getValue(void *struct_address) const = 0;

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


private:
    std::string name;

};