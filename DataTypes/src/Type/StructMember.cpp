#include <sstream>
#include <iostream>

#include "Type/StructMember.hpp"
#include "DataTypeInator.hpp"



// CONSTRUCTOR
StructMember::StructMember(std::string memberName, std::string typeSpecName) 
                        : name(memberName), typeSpecName(typeSpecName), is_valid(false), subType(NULL) {}

// Rule of 3 - we own the datatype, gotta manage it
StructMember::StructMember( const StructMember& original ) {
    is_valid = original.is_valid;
    name = original.name;
    typeSpecName = original.typeSpecName;


    if (original.subType != NULL) {
        subType = original.subType->clone();
    } else {
        subType = NULL;
    }
}

StructMember::~StructMember() {
    if (subType != NULL) {
        delete subType;
    }
}


bool StructMember::validate(const DataTypeInator * dataTypeInator) {
    if (!is_valid) {
        subType = dataTypeInator->resolve(typeSpecName);
        if (subType != NULL) {
            is_valid = true;
        }
    }

    return is_valid;
}

bool StructMember::isValid() const {
    return is_valid;
}

std::string StructMember::getName() const {
    return name;
}

const DataType * StructMember::getSubType() const {
    return subType;
}

