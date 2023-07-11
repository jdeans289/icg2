#include <sstream>
#include <string.h> // for memcpy

#include "Type/Types.hh"
#include "MemoryManagement/AllocInfo.hh"
#include "Utils/MutableDeclaration.hh"



const std::string AllocInfo::localPrefix = "trick_local_";
const std::string AllocInfo::externPrefix = "trick_extern_";
unsigned int AllocInfo::nextSerialNumber = 0;

// PRIVATE MEMBER FUNCTION
void AllocInfo::initialize( const std::string& varName,
                            const DataType*    type,
                            void*              suppliedAllocation)  {

    // Because they can be resized, all allocations are really arrays,
    // whether they have 0, 1, or many elements.
    // uhhhhhh no -jackie
    
    name = varName;
    dataType = type;

    if ( suppliedAllocation != 0 ) {
        start = suppliedAllocation;
        storageClass = StorageClass::EXTERN;
    } else {
        start = dataType->createInstance(1);
        storageClass = StorageClass::LOCAL;
    }

    serialNumber = nextSerialNumber++;

    end = (char*)start + dataType->getSize();
}


// CONSTRUCTOR
AllocInfo::AllocInfo( const std::string& varName,
                      const DataType*    type,
                      void*              suppliedAllocation)  {

    initialize(varName, type, suppliedAllocation);
}


// DESTRUCTOR
AllocInfo::~AllocInfo() {
    // FIXME: what about the allocation?
}

// PUBLIC MEMBER FUNCTION
std::string AllocInfo::getName() const {
    return name;
}

// PUBLIC MEMBER FUNCTION
std::string AllocInfo::getSerialName() const {
    std::ostringstream oss;
    if (storageClass == StorageClass::LOCAL) {
        oss << localPrefix << serialNumber ;
    } else {
        oss << externPrefix << serialNumber ;
    }
    return oss.str();
}

// PUBLIC MEMBER FUNCTION
unsigned int AllocInfo::getSerialNumber() const {
    return serialNumber;
}

// PUBLIC MEMBER FUNCTION
void* AllocInfo::getStart() const {
    return start;
}

// PUBLIC MEMBER FUNCTION
int AllocInfo::getSize() const {
    return dataType->getSize();
}

// PUBLIC MEMBER FUNCTION
std::string AllocInfo::getTypeSpecifierName() const {
    return dataType->getTypeSpecName();
}

// PUBLIC MEMBER FUNCTION
const DataType * AllocInfo::getDataType() const {
    return dataType;
}

// PUBLIC MEMBER FUNCTION
StorageClass::e AllocInfo::getStorageClass() const {
    return storageClass;
}

// PUBLIC MEMBER FUNCTION
void AllocInfo::clear() const {
    dataType->clearValue(start);
}

// // PUBLIC MEMBER FUNCTION
// void* AllocInfo::resize( size_t newElementCount ) {

//     // We should not attempt to resize memory objects that were supplied
//     // to us. We should only attempt to resize objects that we created.
//     if ( storageClass != StorageClass::LOCAL ) {
//         // ERROR CONDITION
//         return NULL;
//     }
//     ArrayDataType* oldType = ownDataType;
//     void * oldMemoryObject = start;
//     size_t oldByteCount = oldType->getSize();

//     ArrayDataType* newType = new ArrayDataType( *oldType, newElementCount );
//     void * newMemoryObject = newType->createInstance(1);
//     size_t newByteCount = newType->getSize();

//     size_t minByteCount;
//     if ( oldByteCount < newByteCount ) {
//         minByteCount = oldByteCount;
//     } else {
//         minByteCount = newByteCount;
//     }

//     if (minByteCount > 0) {
//         memcpy( newMemoryObject, oldMemoryObject, minByteCount);
//     }

//     delete oldType;
//     oldType->deleteInstance( oldMemoryObject );
//     start = newMemoryObject;
//     end = (char*)start + newType->getSize();
//     ownDataType = newType;
//     dataType = newType;
//     dimensions[0] = newElementCount;
//     return start;
// }

// PUBLIC MEMBER FUNCTION
bool AllocInfo::contains(void* address) const {

    if (( address >= start) && (address <= end)) {
        return true;
    }
    return false;
}


// PUBLIC MEMBER FUNCTION
std::string AllocInfo::toString() const {

    std::ostringstream oss;
    if (storageClass == StorageClass::LOCAL) {
        oss << "LOCAL ";
    } else {
        oss << "EXTERN ";
    }
    oss << "<" << getDataType()->makeDeclaration( name) << ">";
    oss << " @";
    oss << std::hex << start << ".." ;
    oss << std::hex << end   << " " ;
    oss << "\tValue: ";
    getDataType()->printValue(oss, start);
    oss << std::endl;
    return oss.str();
}

#ifdef NEWSTUFF
// PUBLIC MEMBER FUNCTION
bool AllocInfo::getVarAccessInfo( LexicalAnalyzer* lexer, VarAccessInfo& varAccessInfo ) {

     bool errorCondition = false;

     if (dimensions[0] == 1) {
         searchType = dataType->subType();
     } else {
         searchType = dataType;
     }

     TypeClass::e typeClass = searchType->getTypeClass();

     nextToken = lexer->getToken();
     if (nextToken = Token::Identifier) {
         std::string temp = lexer->getText();
         if (temp != name) {
              errorCondition = true;
         }
         nextToken = lexer->getToken();
     }

     if (nextToken == Token::EndOfFile) {
         varAccessInfo.dataType = dataType;
         varAccessInfo.address = start;
     } else if ( nextToken == Token::period) {
         if (typeClass == TypeClass::COMPOSITE) {
              errorCondition |= ((const CompositeDataType*)searchType)->getMemberInfo( lexer, start, varAccessInfo);
         } else {
              errorCondition = true;
         }
     } else if (nextToken == Token::LeftBracket) {
         if ( typeClass == TypeClass::ARRAY ) {
             errorCondition |= ((const ArrayDataType*)searchType)->getElementInfo( lexer, start, varAccessInfo);
         } else {
             errorCondition = true;
         }
     } else {
         errorCondition = true;
     }
     return errorCondition;
}
#endif
