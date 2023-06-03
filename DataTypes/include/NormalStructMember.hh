#ifndef NORMAL_STRUCT_MEMBER_H
#define NORMAL_STRUCT_MEMBER_H

#include "TypedStructMember.hh"
#include "DataType.hh"
#include "DataTypeInator.hh"
#include <string>

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
    StructMember * clone () const;

    /**
     Destructor.
     */
    ~NormalStructMember();

    /**
    */
    MemberClass::e getMemberClass() const {
        return MemberClass::NORMAL;
    }

    /**
     */
    int getOffset();

    /**
     */
    void clearValue(void *struct_address) const ;

    /**
     */
    void assignValue(void *struct_address, Value *v) const ;

    /**
     */
    void printValue(std::ostream &s, void *struct_address) const ;

    
    void checkpointValue(std::ostream &s, std::string var_name, void *address) const;

    /**
     Product a string representation of this NormalStructMember.
     */
    std::string toString() const ;

private:
    int byte_offset;
};
#endif
