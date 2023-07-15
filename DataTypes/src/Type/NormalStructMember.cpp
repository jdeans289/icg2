#include <sstream>
#include <iostream>

#include "Type/NormalStructMember.hpp"
#include "DataTypeInator.hpp"


// CONSTRUCTOR
NormalStructMember::NormalStructMember(std::string memberName,
                                       int offset,
                                       DataTypeInator* dataTypeInator,
                                       std::string typeSpecName ) : TypedStructMember(memberName, dataTypeInator, typeSpecName), byte_offset(offset) {}

// CONSTRUCTOR
NormalStructMember::NormalStructMember ( const NormalStructMember & original )
    : TypedStructMember( original ) {

    byte_offset = original.byte_offset;
}

StructMember * NormalStructMember::clone () const {
    return new NormalStructMember( *this );
}

NormalStructMember::~NormalStructMember() {}


// MEMBER FUNCTION
int NormalStructMember::getOffset() const {
    return byte_offset;
}

// MEMBER FUNCTION
void NormalStructMember::clearValue(void *struct_address ) const {

    if (isValid) {
        void * member_address = (char*)struct_address + byte_offset;
        subType->clearValue( member_address);
    } else {
        std::cerr << "ERROR: Attempt to clear a value via unvalidate DataType." << std::endl;
    }
}

// MEMBER FUNCTION
void NormalStructMember::assignValue(void *struct_address, Value *v ) const {

    if (isValid) {
        void * member_address = (char*)struct_address + byte_offset;
        subType->assignValue( member_address, v);
    } else {
        std::cerr << "ERROR: Attempt to assign a value via unvalidate DataType." << std::endl;
    }
}

Value * NormalStructMember::getValue(void *struct_address) const {
    if (isValid) {
        void * member_address = (char*)struct_address + byte_offset;
        return subType->getValue( member_address);
    } else {
        std::cerr << "ERROR: Attempt to assign a value via unvalidated DataType." << std::endl;
        return NULL;
    }
}


void * NormalStructMember::getAddress(void * struct_address) const {
    // Struct address is unused, but we don't care
    return (char*)struct_address + byte_offset;;
}

// MEMBER FUNCTION
std::string NormalStructMember:: toString() const {
    return subType->makeDeclaration(getName());
}
