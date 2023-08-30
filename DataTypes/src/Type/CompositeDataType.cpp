#include "DataTypeInator.hpp"

#include "Type/CompositeDataType.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>

// CONSTRUCTOR
CompositeDataType::CompositeDataType( std::string name,
                                      size_t structSize,
                                      void* (*allocator)(int),
                                      void (*deAllocator)(void*) ) {
    this->is_valid = false;
    this->name = name;
    this->structSize = structSize;
    this->allocator = allocator;
    this->deAllocator = deAllocator;
}



// DESTRUCTOR
CompositeDataType::~CompositeDataType() {}


bool CompositeDataType::validate(DataTypeInator * dataTypeInator) {
    if (!is_valid) {
        is_valid = true;

        // Find types of all members
        for (auto& member : getMemberMap()) {
            auto subType = dataTypeInator->resolve(member.second.getTypeSpecName());
            if (subType == NULL) {
                is_valid = false;
            } else {
                member.second.setSubType(subType);
            }
        }

        // Make sorted_member list
        for (auto& it : getMemberMap()) {
            sorted_members.push_back(&it.second);
        }

        auto offset_comp = [](const StructMember * a, const StructMember * b) -> bool {
            return a->getAddressOfMember() < b->getAddressOfMember();
        };

        // Sort by offset
        std::sort(sorted_members.begin(), sorted_members.end(), offset_comp);
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
            std::cerr << "Warning: CompositeDataType " << getTypeSpecName() << " was initialized with an allocator, but not a deallocator." << std::endl;
        }
    } else {
        free(address);
    }
}

// MEMBER FUNCTION
std::string CompositeDataType::toString() const {
   std::stringstream ss;
   ss << "composite {\n";
   for (const auto member : getSortedMemberList()) {
       ss << member->toString();
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
    return visitor->visitCompositeType(std::static_pointer_cast<const CompositeDataType>(shared_from_this()));
}

bool CompositeDataType::hasMemberNamed(std::string name) {
    return getMemberMap().find(name) != getMemberMap().end();
}

const CompositeDataType::SortedMemberList& CompositeDataType::getSortedMemberList () const {
    return sorted_members;
}

