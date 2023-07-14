#pragma once

#include <string>

#include "Type/TypedStructMember.hpp"
#include "Type/DataType.hpp"
#include "DataTypeInator.hpp"

/**
 */
class NormalStructMember : public TypedStructMember {

public:

    /**
     Constructor.
     */
    NormalStructMember( std::string member_name,
                        int offset,
                        DataTypeInator* dataTypeInator,
                        std::string typeSpecName );

    /**
     Copy Constructor.
    */
    NormalStructMember ( const NormalStructMember & original );

    /**
     Clone.
     */
    StructMember * clone () const override;

    /**
     Destructor.
     */
    ~NormalStructMember();

    /**
    */
    MemberClass::e getMemberClass() const override{
        return MemberClass::NORMAL;
    }

    /**
     */
    int getOffset() const;

    /**
     */
    void clearValue(void *struct_address) const override;

    /**
     */
    void assignValue(void *struct_address, Value *v) const override ;

    Value * getValue(void *struct_address) const override;
    
    void * getAddress(void * struct_address) const override;

    /**
     Product a string representation of this NormalStructMember.
     */
    std::string toString() const ;

private:
    int byte_offset;
};
