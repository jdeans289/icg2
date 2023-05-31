#include "NormalStructMember.hh"
#include "DataTypeInator.hh"
#include "ArrayDataType.hh"
#include <sstream>
#include <iostream>

// CONSTRUCTOR
NormalStructMember::NormalStructMember(std::string memberName,
                                       int offset,
                                       DataTypeInator* dataTypeInator,
                                       std::string typeSpecName ) : StructMember(memberName, dataTypeInator, typeSpecName) {

   byte_offset = offset;
}

// CONSTRUCTOR
NormalStructMember::NormalStructMember ( const NormalStructMember & original )
    :StructMember( original ) {

    byte_offset = original.byte_offset;
}

StructMember * NormalStructMember::clone () const {
    return new NormalStructMember( *this );
}

NormalStructMember::~NormalStructMember() {}


// MEMBER FUNCTION
int NormalStructMember::getOffset() {
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

// MEMBER FUNCTION
void NormalStructMember::printValue(std::ostream &s, void *struct_address) const {
    if (isValid) {
        void * member_address = (char*)struct_address + byte_offset;
        subType->printValue(s, member_address) ;
    } else {
        std::cerr << "ERROR: Attempt to print a value via unvalidate DataType." << std::endl;
    }
}

// void NormalStructMember::checkpointValue(std::ostream &s, std::string var_name, void *struct_address) const {
//     if (isValid) {
//         std::string full_member_name = var_name + "." + getName();
//         void * member_address = (char*)struct_address + byte_offset;
//         subType->checkpointValue(s, full_member_name, member_address);
//     } else {
//         std::cerr << "ERROR: Attempt to checkpoint variable " << var_name << " via unvalidated DataType." << std::endl;
//     }
// }

// MEMBER FUNCTION
std::string NormalStructMember:: toString() const {
    return subType->makeDeclaration(getName());
}
