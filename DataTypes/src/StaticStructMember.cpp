#include "StaticStructMember.hh"
#include "DataTypeInator.hh"
#include "ArrayDataType.hh"
#include <sstream>
#include <iostream>

// CONSTRUCTOR
StaticStructMember::StaticStructMember(std::string memberName,
                                       void * memberAddress,
                                       DataTypeInator* dataTypeInator,
                                       std::string typeSpecName
                                        ) : StructMember(memberName) {


    this->typeSpecName = typeSpecName;

   subType = NULL;
   this->memberAddress = memberAddress;
   this->dataTypeInator = dataTypeInator;
   is_valid = false;
}

// CONSTRUCTOR
StaticStructMember::StaticStructMember ( const StaticStructMember & original )
    :StructMember( original ) {

    memberAddress = original.memberAddress;
    is_valid = original.is_valid;
    typeSpecName = original.typeSpecName;

    if (is_valid) {
        subType = original.subType;
    }
}

StructMember * StaticStructMember::clone () const {
    return new StaticStructMember( *this );
}

StaticStructMember::~StaticStructMember() {
}

bool StaticStructMember::validate() {

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
bool StaticStructMember::containsPointers() const {
    if (is_valid) {
        return subType->containsPointers();
    }
    return false;
}

// MEMBER FUNCTION
const DataType * StaticStructMember::getDataType() {
    return subType;
}

// MEMBER FUNCTION
void* StaticStructMember::getAddress() {
    return memberAddress;
}

// MEMBER FUNCTION
void StaticStructMember::clearValue(void *struct_address) const {

    if (is_valid) {
        subType->clearValue( memberAddress);
    } else {
        std::cerr << "ERROR: Attempt to clear a value via unvalidated DataType." << std::endl;
    }
}

// MEMBER FUNCTION
void StaticStructMember::assignValue(void *struct_address, Value *v ) const {

    if (is_valid) {
        subType->assignValue( memberAddress, v);
    } else {
        std::cerr << "ERROR: Attempt to assign a value via unvalidated DataType." << std::endl;
    }
}

// MEMBER FUNCTION
void StaticStructMember::printValue(std::ostream &s, void *struct_address) const {

    // Note that struct_address is not used. We still need to satisfy the virtual interface.
    if (is_valid) {
        subType->printValue(s, memberAddress) ;
    } else {
        std::cerr << "ERROR: Attempt to print a value via unvalidated DataType." << std::endl;
    }
}

// MEMBER FUNCTION
std::string StaticStructMember:: toString() const {
    std::ostringstream oss;
    oss << "static ";
    oss << subType->makeDeclaration(getName());
    return oss.str();
}
