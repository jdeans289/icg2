#include "Algorithm/FindLeaves.hpp"

#include "Type/VisitableTypes.hpp"
#include "Type/NormalStructMember.hpp"

#include "Algorithm/DataTypeAlgorithm.hpp"

namespace FindLeaves {

    FindLeavesVisitor::FindLeavesVisitor() {}

    FindLeavesVisitor::FindLeavesVisitor(std::string starting_name, void * starting_address) {
        current_name_stack.pushName(starting_name);
        address_stack.push(starting_address);
    }

    // Harvest all the values from the tree

    bool FindLeavesVisitor::visitPrimitiveDataType(const PrimitiveDataType * node) {
        // Add to the leaf stack
        leaves.emplace_back(current_name_stack, DataTypeAlgorithm::getValue(node, address_stack.top()));
        return true;
    }

    bool FindLeavesVisitor::visitStringType(const StringDataType * node) {
        // Add to the leaf stack
        leaves.emplace_back(current_name_stack, DataTypeAlgorithm::getValue(node, address_stack.top()));

        return true;
    }

    bool FindLeavesVisitor::visitCompositeType(const CompositeDataType * node) {
        for (auto it = node->getNormalMemberListBegin(); it != node->getNormalMemberListEnd(); it++) {
            NormalStructMember * member = *it;
            
            const DataType * member_subtype = member->getSubType();
            // Push the member name on stack
            
            current_name_stack.pushName(member->getName());
            address_stack.push(member->getAddress(address_stack.top()));

            // Go into member
            member_subtype->accept(this);

            // Remove member name from stack
            current_name_stack.pop_back();
            address_stack.pop();
        }

        // TODO: BITFIELD AND STATICS NOT IMPLEMENTED YET


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
        // Use the special constructor for a leaf that contains info to resolve a pointer
        leaves.emplace_back(current_name_stack, DataTypeAlgorithm::getValue(node, address_stack.top()), true, node->getSubType());

        return true;
    }

    bool FindLeavesVisitor::visitEnumeratedType(const EnumDataType * node) {
        // An enum is a leaf type

        // Add this name to the leaf stack
        leaves.emplace_back(current_name_stack, DataTypeAlgorithm::getValue(node, address_stack.top()));

        return true;
    }

    bool FindLeavesVisitor::visitSequenceType (const SequenceDataType * node) {
        // A sequence must register its size as a leaf!
        current_name_stack.pushName("size");
        int stl_size = node->getNumElements(address_stack.top());
        Leaf size_leaf (current_name_stack, new IntegerValue(stl_size));
        current_name_stack.pop_back();

        size_leaf.is_stl = true;
        size_leaf.stl_size = stl_size;

        leaves.push_back(size_leaf);

        // Continue traversal
        const DataType * subType = node->getSubType();
        auto elem_addresses = node->getElementAddresses(address_stack.top());

        for (int i = 0; i < elem_addresses.size(); i++) {
            current_name_stack.pushIndex(i);
            address_stack.push( elem_addresses[i] );

            subType->accept( this );
            
            address_stack.pop();
            current_name_stack.pop_back();
        }

        return true;
    }

    Result FindLeavesVisitor::getResult() {
        return leaves;
    }
}