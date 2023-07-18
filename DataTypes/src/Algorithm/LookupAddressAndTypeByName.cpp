#include <cassert>

#include "Algorithm/LookupAddressAndTypeByName.hpp"

#include "Type/Types.hpp"
#include "Type/NormalStructMember.hpp"

namespace LookupAddressAndTypeByName {

    LookupAddressAndTypeByNameVisitor::LookupAddressAndTypeByNameVisitor(void * starting_address, std::string full_name) 
            : current_search_address(starting_address), name_elems(full_name) {}

    LookupAddressAndTypeByNameVisitor::LookupAddressAndTypeByNameVisitor(void * starting_address, MutableVariableName name_elems) 
            : current_search_address(starting_address), name_elems(name_elems) {}

    // Look for the matching address

    bool LookupAddressAndTypeByNameVisitor::visitCompositeType(const CompositeDataType * node) {
        if (name_elems.empty()) {
            // Yay we found our result!

            // Put the current address and type into the result
            result.success = true;
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


        // Find the next step
        const DataType * next_type = NULL;
        for (auto it = node->getNormalMemberListBegin(); it != node->getNormalMemberListEnd(); it++) {
            NormalStructMember * member = *it;
            if (member->getName() == next_elem) {
                // Found the next step!
                // Find the address of the next member
                current_search_address = member->getAddress(current_search_address);
                next_type = member->getSubType();
            }
        }

        for (auto it = node->getStaticMemberListBegin(); it != node->getStaticMemberListEnd(); it++) {
            StaticStructMember * member = *it;
            if (member->getName() == next_elem) {
                // Found the next step!
                // Find the address of the next member
                current_search_address = member->getAddress();
                next_type = member->getSubType();
            }
        }

        if (next_type != NULL) {
            return next_type->accept(this);
        }

        std::cerr << "Could not find member named " << next_elem << " in type " << node->toString() << std::endl;
        return false;
    }

    bool LookupAddressAndTypeByNameVisitor::visitArrayType(const ArrayDataType * node) {
        if (name_elems.empty()) {
            // Yay we found our result!

            // Put the current address and type into the result
            result.success = true;
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

    bool LookupAddressAndTypeByNameVisitor::visitPointerType(const PointerDataType * node) {
        // We're at a leaf, so if there's anything left in the name queue something has gone wrong
        // what even is a pointer anyway

        if (visitLeaf(node)) {
            result.isPointer = true;
            return true;
        } else {
            return false;
        }

    }

    bool LookupAddressAndTypeByNameVisitor::visitPrimitiveDataType(const PrimitiveDataType * node) {
        return visitLeaf(node);
    }

    bool LookupAddressAndTypeByNameVisitor::visitEnumeratedType(const EnumDataType * node) {
        return visitLeaf(node);
    }

    bool LookupAddressAndTypeByNameVisitor::visitStringType(const StringDataType * node) {
        return visitLeaf(node);
    }

    bool LookupAddressAndTypeByNameVisitor::visitLeaf(const DataType * node) {
        // We're at a leaf, so if there's anything left in the name queue something has gone wrong
        if (!name_elems.empty()) {
            std::cerr << "At a leaf type, but there are still name elements left to find." << std::endl;
            return false;
        }

        // Yay we found our result!

        // Put the current address and type into the result
        result.success = true;
        result.type = node;
        result.address = current_search_address;
        
        return true;
    }

    LookupAddressAndTypeByName::Result LookupAddressAndTypeByNameVisitor::getResult() {
        return result;
    }

}