#include <sstream>
#include <iostream>

#include "Type/StructMember.hpp"
#include "DataTypeInator.hpp"



StructMember::StructMember(std::string memberName, std::string typeSpecName, int address, StorageClass storage) 
            : name(memberName), typeSpecName(typeSpecName), storage(storage), memberAddress(address), subType(NULL) {}


StructMember::~StructMember() {}

std::string StructMember::toString() const {
    std::ostringstream oss;
    if (storage == STATIC) {
        oss << "static ";
    }

    if (getSubType() != NULL ) {
        oss << getSubType()->makeDeclaration(getName());
    } else {
        oss << "<unvalidated type>";
    }
    return oss.str();
}

void * StructMember::getAddressOfMember (void * struct_addr) const {
    if (storage == STATIC) {
        return reinterpret_cast<void *>(memberAddress);
    }

    return (char*)struct_addr + memberAddress;
}

