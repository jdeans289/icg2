#include <cassert>

#include "Algorithm/LookupAddressByNameVisitor.hh"

#include "Type/DataType.hh"
#include "Type/CompositeDataType.hh"
#include "Type/ArrayDataType.hh"
#include "Type/PointerDataType.hh"
#include "Type/EnumDataType.hh"
#include "Type/NormalStructMember.hh"

LookupAddressByNameVisitor::LookupAddressByNameVisitor(void * starting_address, std::string full_name) 
        : current_search_address(starting_address), name_elems(full_name) {}

LookupAddressByNameVisitor::LookupAddressByNameVisitor(void * starting_address, MutableVariableName name_elems) 
        : current_search_address(starting_address), name_elems(name_elems) {}

// Look for the matching address

bool LookupAddressByNameVisitor::visitPrimitiveDataType(const DataType * node) {
    // We're at a leaf, so if there's anything left in the name queue something has gone wrong
    if (!name_elems.empty()) {
        std::cerr << "At a leaf type, but there are still name elements left to find." << std::endl;
        return false;
    }

    // Yay we found our result!

    // Put the current address and type into the result
    result.type = node;
    result.address = current_search_address;
    
    return true;
}

bool LookupAddressByNameVisitor::visitCompositeType(const CompositeDataType * node) {
    if (name_elems.empty()) {
        // Yay we found our result!

        // Put the current address and type into the result
        result.type = node;
        result.address = current_search_address;
        
        return true;
    }

    std::string next_elem = name_elems.pop_front();

    // If the next name elem is an index, we done messed up
    if (next_elem[0] == '[') {
        std::cerr << "Got an index where a name was expected." << std::endl;
        return false;
    }

    for (int i = 0; i < node->getMemberCount(); i++) {
        StructMember * member = node->getStructMember(i);
        if (member->getName() == next_elem) {
            // Found the next step!
            if (member->getMemberClass() != MemberClass::BITFIELD) {
                // This will handle static or normal members
                TypedStructMember * typed_member = dynamic_cast<TypedStructMember *> (member);

                // Find the address of the next member
                current_search_address = typed_member->getAddress(current_search_address);

                // Continue the visit!!
                const DataType * next_type = typed_member->getDataType();
                assert (next_type != NULL);
                return next_type->accept(this);

            } else {
                // TODO: figure this out
                // bitfields are scary
                std::cerr << "Found a bitfield but I'm too scared to go any further" << std::endl;
            }
        }
    }

    std::cerr << "Could not find member named " << next_elem << " in type " << node->toString() << std::endl;
    return false;
}

bool LookupAddressByNameVisitor::visitArrayType(const ArrayDataType * node) {
    if (name_elems.empty()) {
        // Yay we found our result!

        // Put the current address and type into the result
        result.type = node;
        result.address = current_search_address;
        
        return true;
    }

    std::string next_elem = name_elems.pop_front();

    // If the next name elem is not an index, we done messed up
    if (next_elem[0] != '[') {
        std::cerr << "Got a name where an index was expected." << std::endl;
        return false;
    }

    // Trim off the brackets and parse as int
    size_t index = stoi (next_elem.substr(1, next_elem.size()-1));
    if (index >= node->getElementCount()) {
        std::cerr << "Cannot get index " << index << " in an array of type " << node->toString() << std::endl;
        return false;
    }

    const DataType * subType = node->getSubType();

    // Set the next search address
    // Just pretend our current_search_address isn't void for a sec
    current_search_address = (char *)current_search_address + subType->getSize() * index;
    return subType->accept(this);
}

bool LookupAddressByNameVisitor::visitPointerType(const PointerDataType * node) {
    // We're at a leaf, so if there's anything left in the name queue something has gone wrong
    // TODO: OR IS IT????????
    // what even is a pointer anyway

    if (!name_elems.empty()) {
        std::cerr << "At a leaf type, but there are still name elements left to find." << std::endl;
        return false;
    }

    // Yay we found our result!

    // Put the current address and type into the result
    result.type = node;
    result.address = current_search_address;
    
    return true;
}

bool LookupAddressByNameVisitor::visitEnumeratedType(const EnumDataType * node) {
    // We're at a leaf, so if there's anything left in the name queue something has gone wrong
    if (!name_elems.empty()) {
        std::cerr << "At a leaf type, but there are still name elements left to find." << std::endl;
        return false;
    }

    // Yay we found our result!

    // Put the current address and type into the result
    result.type = node;
    result.address = current_search_address;
    
    return true;
}

LookupAddressByNameVisitor::Result LookupAddressByNameVisitor::getResult() {
    return result;
}
