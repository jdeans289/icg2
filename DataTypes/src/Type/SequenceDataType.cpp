#include <iostream>
#include <sstream>
#include <cassert>

#include "Type/SequenceDataType.hpp"

#include "Utils/ParsedDeclaration.hpp"

// CONSTRUCTOR
SequenceDataType::SequenceDataType( std::string typeSpecName, int typeSize, void* (*allocator)(int), void (*deAllocator)(void*)) : 
            is_valid(false), typeSpecName(typeSpecName), typeSize(typeSize), subType(NULL), allocator(allocator), deAllocator(deAllocator) {}


/* ================================================================================= */
/*                         RULE OF THREE (and a half) INTERFACE                      */
/* ================================================================================= */
SequenceDataType::SequenceDataType ( SequenceDataType const & original) {
    is_valid = original.is_valid;
    typeSpecName = original.typeSpecName;
    elemTypeName = original.elemTypeName;

    if (original.subType != NULL) {
        subType = original.subType->clone();
    } else {
        subType = NULL;
    }
}



// DESTRUCTOR
SequenceDataType::~SequenceDataType () {
    if ( subType != NULL ) {
        delete subType;
    }
}

// SWAP
void swap(SequenceDataType& a, SequenceDataType& b) 
{
    // enable ADL
    using std::swap;
    swap(a.is_valid, b.is_valid);
    swap(a.typeSpecName, b.typeSpecName);
    swap(a.elemTypeName, b.elemTypeName);
    swap(a.subType, b.subType);
}


bool SequenceDataType::validate(const DataTypeInator * dataTypeInator) {

    if (!is_valid) {
        // Parse the typespec name to get the type of the elements
        ParsedDeclaration parsedDeclaration(typeSpecName);
        auto templateParams = parsedDeclaration.getTemplateParams();
        if (templateParams.size() != 1) {
            std::cerr << "ERROR: Type \"" << typeSpecName << "\" could not be processed as an STL sequence type." << std::endl;
            return false;
        }

        elemTypeName = templateParams[0].getTypeSpecifier();

        // Look up the typename in the dataTypeInator
        subType = dataTypeInator->resolve( elemTypeName );

        if (subType == NULL) {
            std::cerr << "ERROR: Type \"" << elemTypeName << "\" not found." << std::endl;
        } else {
            is_valid = true;
        }
    }
    return is_valid;
}

bool SequenceDataType::isValid() const {
    return is_valid;
}


size_t SequenceDataType::getSize() const {

    if (is_valid) {
        return typeSize;
    } else {
        std::cerr << "ERROR: Can't getSize() because type hasn't been validated." << std::endl;
        return 0;
    }
}

void* SequenceDataType::createInstance(unsigned int num) const {
    return allocator(num);
}

void SequenceDataType::deleteInstance(void* address) const {
    deAllocator(address);
}


std::string SequenceDataType::getTypeSpecName() const {
    return typeSpecName;
}

const DataType * SequenceDataType::getSubType() const {
    return subType;
}


std::string SequenceDataType::makeDeclaration(std::string declarator) const {
    return typeSpecName;
}

bool SequenceDataType::accept (DataTypeVisitor * visitor) const {
    return visitor->visitSequenceType(this);
}
