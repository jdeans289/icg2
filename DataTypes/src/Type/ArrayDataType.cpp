#include <iostream>
#include <sstream>
#include <cassert>

#include "Type/ArrayDataType.hh"
#include "Value/ArrayValue.hh"

#include "Utils/MutableDeclaration.hh"

// CONSTRUCTOR
ArrayDataType:: ArrayDataType( const DataTypeInator* dataTypeInator, std::string typeSpecName, unsigned int n_elems ) : 
                                                    typeSize(0), elementCount(n_elems), is_valid(false), typeSpecName(typeSpecName), 
                                                    dataTypeInator(dataTypeInator) {
    subType = NULL;

    if (elementCount == 0) {
        throw std::logic_error("An ArrayDataType cannot have 0 elements.");
    }
}


// COPY CONSTRUCTOR
ArrayDataType::ArrayDataType ( ArrayDataType const & original) {

    is_valid = original.is_valid;
    typeSpecName = original.typeSpecName;
    elementCount = original.elementCount;
    dataTypeInator = original.dataTypeInator;
    subType = original.subType;
}

DataType * ArrayDataType::clone () const {
    return new ArrayDataType( *this );
}

// DESTRUCTOR
ArrayDataType::~ArrayDataType () {
    if ( typeSpecName.empty()) {
        delete subType;
    }
}

// ASSIGNMENT OPERATOR
ArrayDataType& ArrayDataType::operator=( const ArrayDataType & rhs ) {

    if ( this != &rhs ) {

        // Copy the RHS members before messing with the LHS.
        DataType *clonedDataType;
        if ( rhs.typeSpecName.empty() ) {
            clonedDataType = rhs.subType->clone();
        }

        // Delete any pre-existing LHS members.
        if ( typeSpecName.empty() ) {
            delete subType;
        }

        // Assign the copied RHS members to the LHS.
        elementCount = rhs.elementCount;
        is_valid = rhs.is_valid;
        typeSpecName = rhs.typeSpecName;

        if ( typeSpecName.empty() ) {
            subType = clonedDataType;
        } else {
            subType = rhs.subType;
        }
    }
    return *this;
}

// MEMBER FUNCTION
bool ArrayDataType::validate() {

    if (!is_valid) {

        subType = dataTypeInator->resolve( typeSpecName );


        if (subType == NULL) {
            std::cerr << "ERROR: Type \"" << typeSpecName << "\" not found." << std::endl;
        } else {
            is_valid = true;
            typeSize = elementCount * subType->getSize();
        }
        
    }
    return is_valid;
}

// MEMBER FUNCTION
size_t ArrayDataType::getSize() const {

    if (is_valid) {
        return typeSize;
    } else {
        std::cerr << "ERROR: Can't getSize() because type hasn't been validated." << std::endl;
        return 0;
    }
}

// MEMBER FUNCTION
unsigned int ArrayDataType::getTotalElementCount() const {
    return elementCount;
}

// MEMBER FUNCTION
bool ArrayDataType::containsPointers() const {

    if ((subType->containsPointers())) {
        return true;
    }
    return false;
}

// MEMBER FUNCTION
void* ArrayDataType::createInstance(unsigned int num) const {

    if (subType != NULL) {
        return subType->createInstance( num * elementCount );
    } else {
        std::cerr << "ERROR: Can't create an instance of an unvalidated type." << std::endl;
        return (void*) NULL;
    }
}

// MEMBER FUNCTION
void ArrayDataType::deleteInstance(void* address) const {
    if (subType != NULL) {
        return subType->deleteInstance(address);
    } else {
        std::cout << "WARNING: Can't delete an instance of an unvalidated type." << std::endl;
    }
}

// MEMBER FUNCTION
void ArrayDataType::clearValue(void * address) const {
    if (is_valid) {
        for (unsigned int ii=0; ii < elementCount ; ii++) {
            void * elementAddress = (char*)address + (ii * subType->getSize());
            subType->clearValue( elementAddress );
        }
    } else {
        std::cerr << "ERROR: Type is not yet validated.";
    }
}

// MEMBER FUNCTION
void ArrayDataType::assignValue(void * address, Value*value) const {

    if (is_valid) {
        ArrayValue * array_value_p = dynamic_cast<ArrayValue*> (value);
        if (array_value_p) {
            if (array_value_p->getNumElems() != getElementCount()) {
                std::cerr << "Dimension in ArrayValue (" << array_value_p->getNumElems() << ") does not match dimension of ArrayDataType(" << getElementCount() << ")" << std::endl;
                return;
            }

            for (unsigned int i = 0; i < getElementCount(); i++) {
                void * elemAddress = (char *) address + (i * subType->getSize());
                subType->assignValue(elemAddress, (*array_value_p)[i]);
            }
        }
    }
}

Value * ArrayDataType::getValue(void * address) const {
    if (is_valid) {
        ArrayValue * value = new ArrayValue();
        for (unsigned int i = 0; i < getElementCount(); i++) {
            void * elemAddress = (char *) address + (i * subType->getSize());
            value->pushElem(subType->getValue(elemAddress));
        }
        return value;
    } else {
        std::cerr << "Attempt to getValue via an unvalidated data type." << std::endl;
        return NULL;
    }
}

// MEMBER FUNCTION
void ArrayDataType::printValue(std::ostream &s, void * address) const {

    if (is_valid) {
        s << "{";
        for (unsigned int ii=0; ii < elementCount ; ii++) {
            if (ii) {
            s << ",";
            }
            void * elementAddress = (char*) address + (ii * subType->getSize());
            subType->printValue( s, elementAddress );
        }
        s << "}";
    } else {
        std::cerr << "ERROR: Type is not valid.";
    }
}

// MEMBER FUNCTION
std::string ArrayDataType::getTypeSpecName() const {
    return typeSpecName;
}

// MEMBER FUNCTION
std::string ArrayDataType::makeDeclaration(std::string declarator) const {

    // Parse the typename as a declaration
    MutableDeclaration decl(getTypeSpecName());

    // Push this dimension
    decl.pushDimension(elementCount);

    // Appropriately attach this dimension
    return decl.getDeclarator(declarator);
}

// MEMBER FUNCTION
unsigned int ArrayDataType::getElementCount() const {
    return elementCount;
}

bool ArrayDataType::accept (DataTypeVisitor * visitor) const {
    return visitor->visitArrayType(this);
}

// bool ArrayDataType::lookupVariableNameByOffset(MutableVariableName& nameStack, unsigned int offset, const DataType * expectedType) const {
//     // Look for the name of the variable associated with this offset.

//     if (offset == 0) {
//         // Address found!
        
//         // TODO: compare expected type? Does that make sense to do here?

//         return true;
//     }

//     // If the offset is greater than the size of this array, there's an error.
//     if (offset > getSize()) {
//         std::cerr << "Search offset " << offset << " is greater than the size (" << getSize() << ") of this type (" << toString() << std::endl;
//         return false;
//     }

//     // Figure out which element we should go into
//     unsigned int elem_index = offset / subType->getSize();

//     assert (elem_index < getElementCount());

//     // New offset is the remainder of offset / subtypeSize
//     unsigned int newOffset = offset % subType->getSize();

//     // Push the index onto the stack
//     nameStack.pushIndex(elem_index);

//     // Continue the search in the element
//     return subType->lookupVariableNameByOffset(nameStack, newOffset, expectedType);

// }


#ifdef NEWSTUFF
// MEMBER FUNCTION
bool ArrayDataType::getElementInfo( LexicalAnalyzer* lexer, void* baseAddress, VarAccessInfo& varAccessInfo ) {

    bool errorCondition = false;
    nextToken = lexer->getToken();
    if (nextToken = Token::Integer) {
       int index = std::atoi( (lexer->getText()).c_str() );
    }
    if ( (index >= 0) && (index < elementCount)) {
        const DataType*   elementDataType = subType->getDataType();
        TypeClass::e elementDataTypeClass = subType->getTypeClass();
        void*              elementAddress = (char*) baseAddress + (index * subType->getSize());
        nextToken = lexer->getToken();
        if (nextToken == Token::RightBracket) {

            nextToken = lexer->getToken();
            if (nextToken == Token::EndOfFile) {
                varAccessInfo.dataType = elementDataType;
                varAccessInfo.address  = elementAddress;
            } else if ( nextToken == Token::period) {
                if (elementDataTypeClass == TypeClass::COMPOSITE) {
                    errorCondition |= ((const CompositeDataType*)searchType)->getMemberInfo( lexer, elementAddress, varAccessInfo);
                } else {
                    errorCondition = true;
                }
            } else if (nextToken == Token::LeftBracket) {
                if ( typeClass == TypeClass::ARRAY ) {
                    errorCondition |= ((const ArrayDataType*)searchType)->getElementInfo( lexer, elementAddress, varAccessInfo);
                } else {
                    errorCondition = true;
                }
            } else {
                errorCondition = true;
            }
        } else {
            errorCondition = true;
        }
    } else {
        errorCondition = true;
    }
    return errorCondition;
}
#endif
