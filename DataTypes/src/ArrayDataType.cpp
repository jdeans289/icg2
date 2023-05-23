#include <iostream>
#include <sstream>

#include "ArrayDataType.hh"
#include "ArrayValue.hh"
#include "DeclarationBuilder.hh"
#include "ParsedDeclaration.hh"

// CONSTRUCTOR
ArrayDataType:: ArrayDataType( DataTypeInator* dataTypeInator, std::string typeSpecName, unsigned int n_elems ) : 
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
        for (int ii=0; ii < elementCount ; ii++) {
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

            for (int i = 0; i < getElementCount(); i++) {
                void * elemAddress = (char *) address + (i * subType->getSize());
                subType->assignValue(elemAddress, (*array_value_p)[i]);
            }
        }
    }

    // Why are we using a composite_value to initialize an array type??

    // if (is_valid) {
    //     CompositeValue * composite_value_p = dynamic_cast<CompositeValue*>(value);
    //     if (composite_value_p) {
    //         for (int ii=0; ii < elementCount ; ii++) {
    //             void * elementAddress = (char*)address + (ii * subType->getSize());
    //             subType->assignValue( elementAddress, (*composite_value_p)[ii] );
    //         }
    //     } else {
    //         std::cerr << "ERROR: Value assigned to an array must be a composite value.";
    //     }
    // } else {
    //     std::cerr << "ERROR: Type is not valid for assignment.";
    // }
}

// MEMBER FUNCTION
void ArrayDataType::printValue(std::ostream &s, void * address) const {

    if (is_valid) {
        s << "{";
        for (int ii=0; ii < elementCount ; ii++) {
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
    ParsedDeclaration decl(getTypeSpecName());

    std::vector<int> dims = decl.getDims();

    // Put the current dimension on the front 
    dims.insert(dims.begin(), elementCount);

    // Feed it back to the DeclarationBuilder to appropriately attach this dimension
    DeclarationBuilder builder(decl.getTypeSpecifier(), dims);
    return builder.getDeclarator(declarator);
}

// MEMBER FUNCTION
unsigned int ArrayDataType::getElementCount() const {
    return elementCount;
}

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
