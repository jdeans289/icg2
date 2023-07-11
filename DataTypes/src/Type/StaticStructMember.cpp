#include <sstream>
#include <iostream>

#include "Type/StaticStructMember.hpp"
#include "DataTypeInator.hpp"



// CONSTRUCTOR
StaticStructMember::StaticStructMember(std::string memberName,void * memberAddress,DataTypeInator* dataTypeInator,std::string typeSpecName) 
                                    : TypedStructMember(memberName, dataTypeInator, typeSpecName) {
   this->memberAddress = memberAddress;
}

// CONSTRUCTOR
StaticStructMember::StaticStructMember ( const StaticStructMember & original )
    : TypedStructMember( original ) {

    memberAddress = original.memberAddress;
}

StaticStructMember * StaticStructMember::clone () const {
    return new StaticStructMember( *this );
}

StaticStructMember::~StaticStructMember() {
}


// MEMBER FUNCTION
void* StaticStructMember::getAddress() {
    return memberAddress;
}

// MEMBER FUNCTION
void StaticStructMember::clearValue(void *struct_address) const {

    if (isValid) {
        subType->clearValue( memberAddress);
    } else {
        std::cerr << "ERROR: Attempt to clear a value via unvalidated DataType." << std::endl;
    }
}

// MEMBER FUNCTION
void StaticStructMember::assignValue(void *struct_address, Value *v ) const {

    if (isValid) {
        subType->assignValue( memberAddress, v);
    } else {
        std::cerr << "ERROR: Attempt to assign a value via unvalidated DataType." << std::endl;
    }
}

Value * StaticStructMember::getValue(void *struct_address) const {
    if (isValid) {
        return subType->getValue( memberAddress);
    } else {
        std::cerr << "ERROR: Attempt to assign a value via unvalidated DataType." << std::endl;
        return NULL;
    }
}

void * StaticStructMember::getAddress(void * struct_address) const {
    // Struct address is unused, but we don't care
    return memberAddress;
}



// MEMBER FUNCTION
void StaticStructMember::printValue(std::ostream &s, void *struct_address) const {

    // Note that struct_address is not used. We still need to satisfy the virtual interface.
    if (isValid) {
        subType->printValue(s, memberAddress) ;
    } else {
        std::cerr << "ERROR: Attempt to print a value via unvalidated DataType." << std::endl;
    }
}

// void StaticStructMember::checkpointValue(std::ostream &s, std::string var_name, void *address) const {
//     if (isValid) {
//         std::string full_member_name = var_name + "." + getName();
//         subType->checkpointValue(s, full_member_name, memberAddress);
//     } else {
//         std::cerr << "ERROR: Attempt to checkpoint variable " << var_name << " via unvalidated DataType." << std::endl;
//     }
// }


// MEMBER FUNCTION
std::string StaticStructMember:: toString() const {
    std::ostringstream oss;
    oss << "static ";
    oss << subType->makeDeclaration(getName());
    return oss.str();
}
