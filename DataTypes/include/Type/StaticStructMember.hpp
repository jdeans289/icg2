#pragma once

#include <string>

#include "Type/TypedStructMember.hpp"
#include "DataTypeInator.hpp"

/**
 */
class StaticStructMember : public TypedStructMember {

public:

    /**
     Constructor.
     */
    StaticStructMember( std::string memberName,
                        void * memberAddress,
                        DataTypeInator* dataTypeInator,
                        std::string typeSpecName );

    /**
     Copy Constructor.
    */
    StaticStructMember ( const StaticStructMember & original );

    /**
     Destructor.
     */
    ~StaticStructMember();

    /**
     Clone.
     */
    StaticStructMember * clone () const override;



    /**
    */
    MemberClass::e getMemberClass() const override {
        return MemberClass::STATIC;
    }


    /**
     */
    void* getAddress();

    /**
     */
    void clearValue(void *struct_address) const override;

    /**
     */
    void assignValue(void *struct_address, Value *v) const override;


    Value * getValue(void *struct_address) const override;


    void * getAddress(void * struct_address) const override;

    /**
     Product a string representation of this StaticStructMember.
     */
    std::string toString() const override;

private:
    void * memberAddress;

};
