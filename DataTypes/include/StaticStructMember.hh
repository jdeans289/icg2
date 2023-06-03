#ifndef STATIC_STRUCT_MEMBER_H
#define STATIC_STRUCT_MEMBER_H

#include "TypedStructMember.hh"
#include "DataTypeInator.hh"
#include <string>

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

    /**
     */
    void printValue(std::ostream &s, void *struct_address) const override;

    // void checkpointValue(std::ostream &s, std::string var_name, void *address) const;

    /**
     Product a string representation of this StaticStructMember.
     */
    std::string toString() const override;

private:
    void * memberAddress;

};
#endif
