#include "Algorithm/CheckpointVisitor.hh"

#include "Type/DataType.hh"
#include "Type/CompositeDataType.hh"
#include "Type/ArrayDataType.hh"
#include "Type/PointerDataType.hh"
#include "Type/EnumDataType.hh"
#include "Type/TypedStructMember.hh"

CheckpointVisitor::CheckpointVisitor() {}

CheckpointVisitor::CheckpointVisitor(std::string starting_name, void * starting_address) {
    current_name_stack.pushName(starting_name);
    address_stack.push(starting_address);
}

// Harvest all the values from the tree

bool CheckpointVisitor::visitPrimitiveDataType(const DataType * node) {
    std::cout << "Visiting PrimitiveDataType named " << node->toString() << std::endl;

    // Add to the leaf stack
    leaves.emplace_back(current_name_stack, node->getValue(address_stack.top()));

    return true;
}

bool CheckpointVisitor::visitCompositeType(const CompositeDataType * node) {
    std::cout << "Visiting CompositeDataType named " << node->getTypeSpecName() << std::endl;
    for (int i = 0; i < node->getMemberCount(); i++) {
        StructMember * member = node->getStructMember(i);
        std::cout << "Going into Member named " << member->getName() << std::endl;
        
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
            current_name_stack.pop();
            address_stack.pop();

        } else {
            // TODO: BITFIELD IS NOT IMPLEMENTED YET
            std::cerr << "Found a bitfield member named " << member->getName() << " - skipping for now" << std::endl;
        }
    }

    return true;
}

bool CheckpointVisitor::visitArrayType(const ArrayDataType * node) {
    std::cout << "Visiting ArrayDataType with subtype " << node->getTypeSpecName() << std::endl;

    const DataType * subtype = node->getSubType();
    for (int i = 0; i < node->getElementCount(); i++) {
        std::cout << "Going to element " << i << std::endl;
        void * elemAddress = (char *) address_stack.top() + (i * subtype->getSize());

        // Push element onto stack
        current_name_stack.pushIndex(i);
        address_stack.push(elemAddress);

        subtype->accept(this);

        // Remove element from stack
        current_name_stack.pop();
        address_stack.pop();
    }

    return true;
}

bool CheckpointVisitor::visitPointerType(const PointerDataType * node) {
    // A pointer is a leaf type
    std::cout << "Visiting PointerDataType named " << node->toString() << std::endl;

    // Add this name to the leaf stack
    leaves.emplace_back(current_name_stack, node->getValue(address_stack.top()));

    return true;
}

bool CheckpointVisitor::visitEnumeratedType(const EnumDataType * node) {
    // An enum is a leaf type
    std::cout << "Visiting EnumDataType named " << node->toString() << std::endl;

    // Add this name to the leaf stack
    leaves.emplace_back(current_name_stack, node->getValue(address_stack.top()));

    return true;
}

std::vector <CheckpointVisitor::Leaf> CheckpointVisitor::getResults() {
    return leaves;
}
