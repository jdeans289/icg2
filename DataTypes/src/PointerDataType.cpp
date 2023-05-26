#include "PointerDataType.hh"
#include "DataTypeInator.hh"
#include "PointerValue.hh"
#include "ParsedDeclaration.hh"
#include "DeclarationBuilder.hh"
#include <iostream>
#include <sstream>
#include <stdlib.h> // for free()

// CONSTRUCTOR
PointerDataType::PointerDataType( DataTypeInator * datatypeInator, std::string typeSpecifierName) : typeSpecName(typeSpecifierName), dataTypeInator(datatypeInator) {}

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

// ASSIGNMENT OPERATOR
PointerDataType& PointerDataType::operator=( const PointerDataType & rhs ) {

    if ( this != &rhs ) {
        // Assign the copied RHS members to the LHS.
        // elementCount = rhs.elementCount;
        is_valid = rhs.is_valid;
        typeSpecName = rhs.typeSpecName;
        subType = rhs.subType->clone();
    }
    return *this;
}

// MEMBER FUNCTION
// FIXME: make sure that this method in other DataTypes checks for PointerData types rather than ArrayDataTypes.
bool PointerDataType::containsPointers() const {
    return true;
}

// CLONE
DataType * PointerDataType::clone () const {
    return new PointerDataType( *this );
}

// MEMBER FUNCTION
bool PointerDataType::validate() {

    if (!is_valid) {
        // if we're not referencing a dictionary type.
        // if (typeSpecName.empty()) {
        //     is_valid = ownDataType->validate();
        //     if (is_valid) {
        //         subType = ownDataType;
        //     }
        // } else {
        subType = dataTypeInator->resolve( typeSpecName );
        if (subType != NULL) {
            is_valid = true;
        } else {
            std::cerr << "ERROR: Type \"" << typeSpecName << "\" not found." << std::endl;
        }
        // }
    }
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

// MEMBER FUNCTION
void PointerDataType::clearValue(void * address) const {
    *(void**)address =  (void*)NULL;
}

// MEMBER FUNCTION
void PointerDataType::assignValue(void * address, Value*value) const {

    // sus --jackie

    PointerValue * pointer_value_p = dynamic_cast<PointerValue*>(value);
    if (pointer_value_p) {
        *(void**)address =  pointer_value_p->getPointer();
    } else {
        std::cerr << "ERROR: Attempt to assign non-pointer value to a pointer.";
    }
}

// MEMBER FUNCTION
void PointerDataType::printValue(std::ostream &s, void * address) const {
    s << *(void**)address;
}

// MEMBER FUNCTION
std::string PointerDataType::getTypeSpecName() const {
    return typeSpecName;
}

// MEMBER FUNCTION
std::string PointerDataType::makeDeclaration(std::string declarator) const {
    // Parse the typename as a declaration
    ParsedDeclaration decl(getTypeSpecName());
    
    std::vector<int> dims = decl.getDims();

    // Put the current dimension on the front
    dims.insert(dims.begin(), -1);

    // Feed it back to the DeclarationBuilder to appropriately attach this dimension
    DeclarationBuilder builder(decl.getTypeSpecifier(), dims);
    return builder.getDeclarator(declarator);
}

