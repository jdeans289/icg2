#include <iostream>

#include "StructMember.hh"

// Bad secret constructor
StructMember::StructMember(std::string name) : StructMember(name, NULL, "") {}

// Real constructor
StructMember::StructMember(std::string name,  DataTypeInator* dataTypeInator, std::string typeSpecName) 
                        : typeSpecName(typeSpecName),  subType(NULL), dataTypeInator(dataTypeInator),  isValid(false), name(name) {}


// COPY CONSTRUCTOR
StructMember::StructMember ( const StructMember & original) {
    this->name = original.name;
    this->subType = original.subType;
    this->dataTypeInator = original.dataTypeInator;
    this->typeSpecName = original.typeSpecName;
    this->isValid = original.isValid;

}

std::string StructMember::getName() const {
    return name;
}

bool StructMember::validate() {

    if (!isValid) {
        subType = dataTypeInator->resolve( typeSpecName );

        if (subType != NULL) {
            isValid = true;
        } else {
            std::cerr << "ERROR: Type \"" << typeSpecName
                        << "\" not found in the DataTypeInator." << std::endl;
        }
    }
    return isValid;
}

// MEMBER FUNCTION
bool StructMember::containsPointers() const {
    if (isValid) {
        return subType->containsPointers();
    }
    return false;
}

// MEMBER FUNCTION
const DataType * StructMember::getDataType() {
    return subType;
}



