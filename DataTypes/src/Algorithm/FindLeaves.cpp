#include "Algorithm/FindLeaves.hpp"

#include "Type/Types.hpp"
#include "Type/TypedStructMember.hpp"

namespace FindLeaves {

    FindLeavesVisitor::FindLeavesVisitor() {}

    FindLeavesVisitor::FindLeavesVisitor(std::string starting_name, void * starting_address) {
        current_name_stack.pushName(starting_name);
        address_stack.push(starting_address);
    }

    // Harvest all the values from the tree

    bool FindLeavesVisitor::visitPrimitiveDataType(const PrimitiveDataType * node) {
        // Add to the leaf stack
        leaves.emplace_back(current_name_stack, node->getValue(address_stack.top()));

        return true;
    }

    bool FindLeavesVisitor::visitStringType(const StringDataType * node) {
        // Add to the leaf stack
        leaves.emplace_back(current_name_stack, node->getValue(address_stack.top()));

        return true;
    }

    bool FindLeavesVisitor::visitCompositeType(const CompositeDataType * node) {
        for (int i = 0; i < node->getMemberCount(); i++) {
            StructMember * member = node->getStructMember(i);
            
            // We need to figure out how to correctly handle the bitfields - that's a weird edge case
            TypedStructMember * typed_member = dynamic_cast<TypedStructMember *> (member);

            if (typed_member != NULL) {
                const DataType * member_subtype = typed_member->getDataType();
                // Push the member name on stack
                
                current_name_stack.pushName(member->getName());
                address_stack.push(typed_member->getAddress(address_stack.top()));

                // Go into member
                member_subtype->accept(this);

                // Remove member name from stack
                current_name_stack.pop_back();
                address_stack.pop();

            } else {
                // TODO: BITFIELD IS NOT IMPLEMENTED YET
                std::cerr << "Found a bitfield member named " << member->getName() << " - skipping for now" << std::endl;
            }
        }

        return true;
    }

    bool FindLeavesVisitor::visitArrayType(const ArrayDataType * node) {
        const DataType * subtype = node->getSubType();
        for (int i = 0; i < node->getElementCount(); i++) {
            void * elemAddress = (char *) address_stack.top() + (i * subtype->getSize());

            // Push element onto stack
            current_name_stack.pushIndex(i);
            address_stack.push(elemAddress);

            subtype->accept(this);

            // Remove element from stack
            current_name_stack.pop_back();
            address_stack.pop();
        }

        return true;
    }

    bool FindLeavesVisitor::visitPointerType(const PointerDataType * node) {
        // A pointer is a leaf type
        
        // Add this name to the leaf stack
        leaves.emplace_back(current_name_stack, node->getValue(address_stack.top()), true, node->getSubType());

        return true;
    }

    bool FindLeavesVisitor::visitEnumeratedType(const EnumDataType * node) {
        // An enum is a leaf type

        // Add this name to the leaf stack
        leaves.emplace_back(current_name_stack, node->getValue(address_stack.top()));

        return true;
    }

    Result FindLeavesVisitor::getResult() {
        return leaves;
    }
}