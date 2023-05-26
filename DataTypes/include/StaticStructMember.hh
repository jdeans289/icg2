#ifndef STATIC_STRUCT_MEMBER_H
#define STATIC_STRUCT_MEMBER_H

#include "StructMember.hh"
#include "DataTypeInator.hh"
#include <string>

/**
 */
class StaticStructMember : public StructMember {

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
     Clone.
     */
    StructMember * clone () const;

    /**
     Destructor.
     */
    ~StaticStructMember();

    /**
    */
    MemberClass::e getMemberClass() const {
        return MemberClass::STATIC;
    }


    /**
     */
    void* getAddress();

    /**
     */
    void clearValue(void *struct_address) const;

    /**
     */
    void assignValue(void *struct_address, Value *v) const;

    /**
     */
    void printValue(std::ostream &s, void *struct_address) const;

    void checkpointValue(std::ostream &s, std::string var_name, void *address) const;

    /**
     Product a string representation of this StaticStructMember.
     */
    std::string toString() const;

private:
    void * memberAddress;

};
#endif
