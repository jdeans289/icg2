#include "NormalStructMember.hh"
#include "DataTypeInator.hh"
#include "ArrayDataType.hh"
#include <sstream>
#include <iostream>

// CONSTRUCTOR
NormalStructMember::NormalStructMember(std::string memberName,
                                       int offset,
                                       DataTypeInator* dataTypeInator,
                                       std::string typeSpecName ) : StructMember(memberName) {

    this->typeSpecName = typeSpecName;

   subType = NULL;
   byte_offset = offset;
   is_valid = false;
   this->dataTypeInator = dataTypeInator;
}

// CONSTRUCTOR
NormalStructMember::NormalStructMember ( const NormalStructMember & original )
    :StructMember( original ) {

    byte_offset = original.byte_offset;
    is_valid = original.is_valid;
    typeSpecName = original.typeSpecName;
    subType = original.subType;
}

StructMember * NormalStructMember::clone () const {
    return new NormalStructMember( *this );
}

NormalStructMember::~NormalStructMember() {}

bool NormalStructMember::validate() {

    if (!is_valid) {
        subType = dataTypeInator->resolve( typeSpecName );

        if (subType != NULL) {
            is_valid = true;
        } else {
            std::cerr << "ERROR: Type \"" << typeSpecName
                        << "\" not found in the DataTypeInator." << std::endl;
        }
    }
    return is_valid;
}

// MEMBER FUNCTION
bool NormalStructMember::containsPointers() const {
    if (is_valid) {
        return subType->containsPointers();
    }
    return false;
}

// MEMBER FUNCTION
const DataType * NormalStructMember::getDataType() {
    return subType;
}

// MEMBER FUNCTION
int NormalStructMember::getOffset() {
    return byte_offset;
}

// MEMBER FUNCTION
void NormalStructMember::clearValue(void *struct_address ) const {

    if (is_valid) {
        void * member_address = (char*)struct_address + byte_offset;
        subType->clearValue( member_address);
    } else {
        std::cerr << "ERROR: Attempt to clear a value via unvalidate DataType." << std::endl;
    }
}

// MEMBER FUNCTION
void NormalStructMember::assignValue(void *struct_address, Value *v ) const {

    if (is_valid) {
        void * member_address = (char*)struct_address + byte_offset;
        subType->assignValue( member_address, v);
    } else {
        std::cerr << "ERROR: Attempt to assign a value via unvalidate DataType." << std::endl;
    }
}

// MEMBER FUNCTION
void NormalStructMember::printValue(std::ostream &s, void *struct_address) const {
    if (is_valid) {
        void * member_address = (char*)struct_address + byte_offset;
        subType->printValue(s, member_address) ;
    } else {
        std::cerr << "ERROR: Attempt to print a value via unvalidate DataType." << std::endl;
    }
}

// MEMBER FUNCTION
std::string NormalStructMember:: toString() const {
    return subType->makeDeclaration(getName());
}
