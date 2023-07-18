#include <iostream>
#include <sstream>
#include <stdlib.h> // for free()

#include "Type/PointerDataType.hpp"
#include "Value/PointerValue.hpp"
#include "Utils/MutableDeclaration.hpp"

#include "DataTypeInator.hpp"




// CONSTRUCTOR
PointerDataType::PointerDataType( const DataTypeInator * datatypeInator, std::string typeSpecifierName) : typeSpecName(typeSpecifierName), dataTypeInator(datatypeInator) {}

// COPY CONSTRUCTOR
PointerDataType::PointerDataType ( PointerDataType const & original) {

    is_valid = original.is_valid;
    typeSpecName = original.typeSpecName;
    dataTypeInator = original.dataTypeInator;

    if (original.subType == NULL) {
        subType = NULL;
    } else {
        subType = original.subType->clone();
    }
}

// DESTRUCTOR
PointerDataType::~PointerDataType () {

    if ( typeSpecName.empty()) {
        delete subType;
    }
}

// CLONE
DataType * PointerDataType::clone () const {
    return new PointerDataType( *this );
}

// MEMBER FUNCTION
bool PointerDataType::validate() {

    if (!is_valid) {
        subType = dataTypeInator->resolve( typeSpecName );
        if (subType != NULL) {
            is_valid = true;
        } else {
            std::cerr << "ERROR: Type \"" << typeSpecName << "\" not found." << std::endl;
        }
    }
    return is_valid;
}

bool PointerDataType::isValid() const {
    return is_valid;
}

// MEMBER FUNCTION
size_t PointerDataType::getSize() const {
    return sizeof(void*);
}

// MEMBER FUNCTION
void* PointerDataType::createInstance(unsigned int num) const {
    void* temp = calloc(num, sizeof(void*));
    return temp;
}

// MEMBER FUNCTION
void PointerDataType::deleteInstance(void* address) const {
    free(address);
}

// // MEMBER FUNCTION
// void PointerDataType::clearValue(void * address) const {
//     *(void**)address =  (void*)NULL;
// }



// MEMBER FUNCTION
std::string PointerDataType::getTypeSpecName() const {
    return typeSpecName;
}

// MEMBER FUNCTION
std::string PointerDataType::makeDeclaration(std::string declarator) const {
    // Parse the typename as a declaration
    MutableDeclaration decl(getTypeSpecName());
    
    // Put the current dimension on the front
    decl.pushDimension(-1);

    return decl.getDeclarator(declarator);
}

bool PointerDataType::accept (DataTypeVisitor * visitor) const {
    return visitor->visitPointerType(this);
}

// bool PointerDataType::lookupVariableNameByOffset(MutableVariableName& nameStack, unsigned int offset, const DataType * expectedType) const {
//     // This is a leaf. If offset isn't 0, there's no where else to go so bad luck
//     if (offset != 0) {
//         std::cerr << "Something went wrong - at leaf in search in " << __FILE__ <<  ":" <<  __FUNCTION__ << " but offset is not 0." << std::endl;
//         return false;
//     }

//     // TODO: We need to compare expected type

//     return true;
// }