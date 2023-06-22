#include <cassert>

#include "Algorithm/LookupNameByAddressVisitor.hh"

#include "Type/DataType.hh"
#include "Type/CompositeDataType.hh"
#include "Type/ArrayDataType.hh"
#include "Type/PointerDataType.hh"
#include "Type/EnumDataType.hh"
#include "Type/NormalStructMember.hh"

LookupNameByAddressVisitor::LookupNameByAddressVisitor(std::string starting_name, void * starting_address, void * lookup_address) 
        : LookupNameByAddressVisitor(starting_name, starting_address, lookup_address, NULL) {}

LookupNameByAddressVisitor::LookupNameByAddressVisitor(std::string starting_name, void * starting_address, void * lookup_address, const DataType * const search_type)
        : search_type(search_type) {

    name_stack.pushName(starting_name);
    search_offset = (long) lookup_address - (long) starting_address;

    if (search_offset < 0) {
        // hmmmmmmmmm
        std::cerr << "Something bad happened" << std::endl;
    }
}

// Look for the matching address

bool LookupNameByAddressVisitor::visitPrimitiveDataType(const DataType * node) {
    // std::cout << "Visiting PrimitiveDataType named " << node->toString() << std::endl;

    if (search_offset != 0) {
        std::cerr << "Got to a leaf, but the offset is not 0. Something went wrong. Current name stack: " << name_stack.toString() << "\tCurrent offset: " << search_offset << std::endl;
        return false;
    }

    return typeCheck(node);
}

bool LookupNameByAddressVisitor::visitCompositeType(const CompositeDataType * node) {
    // std::cout << "Visiting CompositeDataType named " << node->getTypeSpecName() << std::endl;

    if (search_offset == 0) {
        // Address found!
        // Check the type and return
        if (typeCheck(node)) {
            return true;
        }

        // Otherwise, let it fall through below to check the first element
    }

    if (search_offset > node->getSize()) {
        std::cerr << "Something went wrong - search offset is larger than composite type.";
        std::cerr << "Current name stack: " << name_stack.toString() << "\tCurrent offset: " << search_offset << std::endl;
        return false;
    }

    // Look for the correct member
    for (int i = 0; i < node->getMemberCount(); i++) {
        StructMember * member = node->getStructMember(i);
        // std::cout << "Going into Member named " << member->getName() << std::endl;
        
        if (member->getMemberClass() == MemberClass::NORMAL) {
            const NormalStructMember * normal_member = dynamic_cast<NormalStructMember *> (member);
            assert(normal_member != NULL);

            const DataType * member_subtype = normal_member->getDataType();
            assert(member_subtype != NULL);

            int member_offset = normal_member->getOffset();
            int member_size = member_subtype->getSize();

            if (search_offset >= member_offset && search_offset < member_offset + member_size) {
                // Found the correct member to go into!

                // Push the member name on stack
                name_stack.pushName(member->getName());
                search_offset = search_offset - member_offset;

                // Go into member
                return member_subtype->accept(this);
            }

            // Otherwise, just keep looking

        } else {
            // TODO: BITFIELD AND STATIC IS NOT IMPLEMENTED YET
            std::string member_class = member->getMemberClass() == MemberClass::BITFIELD ? "bitfield" : "static";
            std::cerr << "Found a " << member_class << "member named " << member->getName() << " - skipping." << std::endl;

            // just keep looking
        }
    }

    // Oh no!
    // If we didn't return already, we couldn't find the search offset
    return false;
}

bool LookupNameByAddressVisitor::visitArrayType(const ArrayDataType * node) {
    // std::cout << "Visiting ArrayDataType with subtype " << node->getTypeSpecName() << std::endl;

    if (search_offset == 0) {
        // Address found!
        // However, it could be the array, or it could be something at the first element of the array.
        // Use the type to check.

        if (typeCheck(node)) {
            // it's this! yay!
            return true;
        } 

        // Otherwise check the first element
    }

    // If the offset is greater than the size of this array, there's an error.
    if (search_offset > node->getSize()) {
        std::cerr << "Search offset " << search_offset << " is greater than the size (" << node->getSize() << ") of this type (" << node->toString() << ")." << std::endl;
        return false;
    }

    const DataType * subType = node->getSubType();

    // Figure out which element we should go into
    unsigned int elem_index = search_offset / subType->getSize();

    if (elem_index >= node->getElementCount()) {
        std::cerr << "Search offset indicates element " << elem_index << " of an array that only has " << node->getElementCount() << " elems." << std::endl;
        return false;
    }

    // New offset is the remainder of offset / subtypeSize
    search_offset = search_offset % subType->getSize();

    // Push the index onto the stack
    name_stack.pushIndex(elem_index);

    // Continue the search in the element
    return subType->accept(this);
}

bool LookupNameByAddressVisitor::visitPointerType(const PointerDataType * node) {
    // A pointer is a leaf type
    // std::cout << "Visiting PointerDataType named " << node->toString() << std::endl;

    if (search_offset != 0) {
        std::cerr << "Got to a leaf, but the offset is not 0. Something went wrong. Current name stack: " << name_stack.toString() << "\tCurrent offset: " << search_offset << std::endl;
        return false;
    }

    // Yay! We found the variable!
    return typeCheck(node);
}

bool LookupNameByAddressVisitor::visitEnumeratedType(const EnumDataType * node) {
    // An enum is a leaf type
    // std::cout << "Visiting EnumDataType named " << node->toString() << std::endl;

    if (search_offset != 0) {
        std::cerr << "Got to a leaf, but the offset is not 0. Something went wrong. Current name stack: " << name_stack.toString() << "\tCurrent offset: " << search_offset << std::endl;
        return false;
    }

    // Yay! We found the variable!
    return typeCheck(node);
}

std::string LookupNameByAddressVisitor::getResult() {
    return name_stack.toString();
}

bool LookupNameByAddressVisitor::typeCheck(const DataType * node) {
    // Check equality by comparing toString
    // This is not great but seems ok
    if (search_type != NULL && node->toString() != search_type->toString()) {
            // std::cout << "Found the search offset at leaf at wrong type. Name:" << name_stack.toString() << std::endl;
            // std::cout << "Expected type: " << search_type->toString() << "\tFound type: " << node->toString() << std::endl;

        // This is not necessarily an error, don't print anything
        return false;
    }        

    // Yay! We found it!
    return true;
}