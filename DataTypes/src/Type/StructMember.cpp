#include <sstream>
#include <iostream>

#include "Type/StructMember.hpp"
#include "DataTypeInator.hpp"



// CONSTRUCTOR
StructMember::StructMember(std::string memberName, std::string typeSpecName) 
                        : name(memberName), typeSpecName(typeSpecName), is_valid(false), subType(NULL) {}


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

