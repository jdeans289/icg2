#pragma once

#include <string>
#include <vector>

#include "Type/StructMember.hpp"

#include "Value/Value.hpp"
#include "DataTypeInator.hpp"

/**
 TypedStructMember is a StructMember that isn't a bitfield
 */
class TypedStructMember : public StructMember {

public:

    /**
     Constructor for StructMember.
     @param name The name of the member.
     @param dataTypeInator Pointer to the DataTypeInator used for resolving types.
     @param typeSpecName string representing the type
     */
    TypedStructMember(std::string name, DataTypeInator* dataTypeInator, std::string typeSpecName);


    /**
    Constructor for StructMember.
    @warning This should ONLY be called by a constructor of a derived class.
    */
    TypedStructMember (const TypedStructMember & original);

    /**
    Destructor.
    */
    virtual ~TypedStructMember () { /* Nothing to do */ }

    /**
     */
    virtual bool validate() override;

    /**
     @return does the DataType or any member of the DataType represent a pointer?
     */
    virtual bool containsPointers() const override;

    const DataType * getDataType() const;

    virtual void * getAddress(void * struct_address) const = 0;



protected:
    std::string typeSpecName;
    const DataType * subType;
    DataTypeInator* dataTypeInator;
    bool isValid;
};
