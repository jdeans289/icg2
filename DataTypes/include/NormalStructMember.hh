#ifndef NORMAL_STRUCT_MEMBER_H
#define NORMAL_STRUCT_MEMBER_H

#include "StructMember.hh"
#include "DataType.hh"
#include "DataTypeInator.hh"
#include <string>

/**
 */
class NormalStructMember : public StructMember {

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
    bool validate();

    /**
    */
    MemberClass::e getMemberClass() const {
        return MemberClass::NORMAL;
    }

    /**
     */
    bool containsPointers() const ;

    /**
      To check for circular reference only.
     */
    const DataType * getDataType();

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

    /**
     Product a string representation of this NormalStructMember.
     */
    std::string toString() const ;

private:
    int byte_offset;
    bool is_valid;
    std::string typeSpecName;
    const DataType * subType;
    DataTypeInator* dataTypeInator;

};
#endif
