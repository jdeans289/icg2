#include "DataTypeInator.hpp"

#include "Type/CompositeDataType.hpp"
#include "Type/NormalStructMember.hpp"
#include "Type/StaticStructMember.hpp"
#include "Type/BitfieldStructMember.hpp"

#include "Value/CompositeValue.hpp"

#include <iostream>

CompositeDataType::CompositeDataType() {

    this->is_valid = false;
    this->structSize = 0;
}

// CONSTRUCTOR
CompositeDataType::CompositeDataType( DataTypeInator* dataTypeInator,
                                      std::string name,
                                      size_t structSize,
                                      void* (*allocator)(int),
                                      void (*deAllocator)(void*) ) {
    this->is_valid = false;
    this->name = name;
    this->structSize = structSize;
    this->allocator = allocator;
    this->deAllocator = deAllocator;
    this->dataTypeInator = dataTypeInator;
}

// CONSTRUCTOR
CompositeDataType::CompositeDataType ( const CompositeDataType & original ) {

    this->structSize = original.structSize;
    this->is_valid = original.is_valid;
    this->name = original.name;
    this->allocator = original.allocator;
    this->deAllocator = original.deAllocator;
    this->dataTypeInator = original.dataTypeInator;

    for (auto member : original.memberList) {
        this->memberList.push_back( member->clone() );
    }
}

// DESTRUCTOR
CompositeDataType::~CompositeDataType() {
    for (auto member : memberList) {
        delete member;
    }
}

// OPERATOR=
CompositeDataType & CompositeDataType::operator=( CompositeDataType rhs)  {
    swap(*this, rhs);
    return *this;
}

// SWAP
void swap(CompositeDataType& a, CompositeDataType& b) 
{
    // enable ADL
    using std::swap;
    swap(a.is_valid, b.is_valid);
    swap(a.name, b.name);
    swap(a.allocator, b.allocator);
    swap(a.deAllocator, b.deAllocator);
    swap(a.dataTypeInator, b.dataTypeInator);
    swap(a.memberList , b.memberList);
    swap(a.structSize , b.structSize);
}

DataType * CompositeDataType::clone() const {
    return new CompositeDataType( *this );
}


bool CompositeDataType::validate() {

    // FIXME: Check for circular dependencies.
    // Also need a to check that this->subType (ptr) is not equal
    // to any of the  member->subType's. Otherwise we will have a circular reference.

    if (!is_valid) {

        is_valid = true;

        int memberCount = memberList.size() ;
        for (int ii=0 ; ii < memberCount ; ii++) {
            is_valid = is_valid && memberList[ii]->validate();
        }

        /* FIXME: Also need to verify that the structSize is at least as great as the max
        member offset plus the size of that member. This may require StructMember to have
        virtual members for getSize and getOffset. Note that this has to work with bitfields. */

    }
    return is_valid;
}

bool CompositeDataType::isValid () const {
    return is_valid;
}

// MEMBER FUNCTION
size_t CompositeDataType::getSize() const {
    return structSize;
}

// MEMBER FUNCTION
void* CompositeDataType::createInstance(unsigned int num) const {

    void* address;
    if (allocator == NULL) {
        // Allocate using calloc.
        if ((address = calloc( (size_t)num, (size_t)structSize ))  == NULL) {
            std::cerr << "ERROR: Out of memory." << std::endl;
            return ((void*)NULL);
        }
    } else {
        // Allocate using the allocator.
        if ((address = allocator( num)) == NULL) {
            std::cerr << "ERROR: Out of memory." << std::endl;
            return ((void*)NULL);
        }
    }
    return address;
}

// MEMBER FUNCTION
void CompositeDataType::deleteInstance(void* address) const {

    if (allocator != NULL) {
        if(deAllocator != NULL) {
            deAllocator(address);
        } else {
            // FIXME: WARNING.
        }
    } else {
        free(address);
    }
}

// MEMBER FUNCTION
void CompositeDataType::clearValue(void * address) const {

    int memberCount = memberList.size() ;
    for (int ii=0; ii < memberCount ; ii++) {
        memberList[ii]->clearValue( address);
    }
}

// MEMBER FUNCTION
void CompositeDataType::assignValue(void * address, Value * value) const {

    // Confirm that the value is a CompositeValue.
    CompositeValue * composite_value = dynamic_cast<CompositeValue*>(value);

    if (composite_value) {

        int memberCount = memberList.size() ;

        int num_values = composite_value->getMemberCount();
        if ( memberCount != num_values) {
            std::cerr << "ERROR: number of members (" << memberCount
                      << ") and number of values (" << num_values
                      << ") is not the same.";
        }

        for (int ii=0; ii < memberCount ; ii++) {
            memberList[ii]->assignValue( address, (*composite_value)[ii]);
        }

    } else {
        std::cerr << "ERROR: Cannot assign a non-composite value to a composite type.";
    }
}

// MEMBER FUNCTION
Value * CompositeDataType::getValue(void * address) const {

    // Confirm that the value is a CompositeValue.
    CompositeValue * ret = new CompositeValue();

    for (StructMember * member : memberList ) {
        ret->addMember(member->getValue(address));
    }

    return ret;

}


// MEMBER FUNCTION
std::string CompositeDataType::toString() const {
   std::stringstream ss;
   int memberCount = memberList.size() ;
   ss << "composite {\n";
   for (int ii=0; ii < memberCount ; ii++) {
       ss << memberList[ii]->toString();
       ss << ";\n";
   }
   ss << "}\n";
   return ss.str();
}

// MEMBER FUNCTION
std::string CompositeDataType::getTypeSpecName() const {
    return name;
}

bool CompositeDataType::accept (DataTypeVisitor * visitor) const {
    return visitor->visitCompositeType(this);
}


// MEMBER FUNCTION
void CompositeDataType::addRegularMember( std::string memberName,
                                               int member_offset,
                                               std::string typeSpecName )  {

    int memberCount = memberList.size() ;
    for (int ii=0 ; ii < memberCount ; ii++) {
        if (memberName == memberList[ii]->getName()) {
            std::stringstream error_stream ;
            error_stream << "ERROR: Attempt to re-define member \"" << memberName << "\"." << std::endl;
            throw std::logic_error( error_stream.str());
        }
    }

    memberList.push_back(
        new NormalStructMember(memberName, member_offset, dataTypeInator, typeSpecName)
    );
}

// MEMBER FUNCTION
void CompositeDataType::addStaticMember( std::string memberName,
                                         void * memberAddress,
                                         std::string typeSpecName )  {

    int memberCount = memberList.size() ;
    for (int ii=0 ; ii < memberCount ; ii++) {
        if (memberName == memberList[ii]->getName()) {
            std::stringstream error_stream ;
            error_stream << "ERROR: Attempt to re-define member \"" << memberName << "\"." << std::endl;
            throw std::logic_error( error_stream.str());
        }
    }

    memberList.push_back(
        new StaticStructMember( memberName, memberAddress, dataTypeInator, typeSpecName )
    );

}


// MEMBER FUNCTION
int CompositeDataType::getMemberCount() const {
    return ( (int)memberList.size() );
}

// MEMBER FUNCTION
StructMember* CompositeDataType::getStructMember (const int index) const {
    if (index >= getMemberCount()) {
        return NULL;
    }
    return ( memberList[index] );
}

StructMember* CompositeDataType::getStructMember (std::string name) const {
    // Lookup struct member by name
    for (auto member : memberList) {
        if (member->getName() == name) {
            return member;
        }
    }

    return NULL;
}