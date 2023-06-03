#include "CheckpointVisitor.hh"

#include "DataType.hh"
#include "CompositeDataType.hh"
#include "ArrayDataType.hh"
#include "PointerDataType.hh"
#include "EnumDataType.hh"
#include "TypedStructMember.hh"


CheckpointVisitor::CheckpointVisitor(CheckpointAgent * agent) : checkpointAgent(agent) {}

void CheckpointVisitor::visitPrimitiveDataType(const DataType * node) {
    std::cout << "Visiting PrimitiveDataType named " << node->toString() << std::endl;
    
}

void CheckpointVisitor::visitCompositeType(const CompositeDataType * node) {
    std::cout << "Visiting CompositeDataType named " << node->getTypeSpecName() << std::endl;
    for (int i = 0; i < node->getMemberCount(); i++) {
        StructMember * member = node->getStructMember(i);
        std::cout << "Going into Member named " << member->getName() << std::endl;
        
        // We need to figure out how to correctly handle the bitfields - that's a weird edge case
        TypedStructMember * typed_member = dynamic_cast<TypedStructMember *> (member);

        if (typed_member != NULL) {
            const DataType * member_subtype = typed_member->getDataType();
            // Push the member name on stack
            checkpointAgent->pushName(member->getName());
            
            // Go into member
            member_subtype->accept(this);

            // Remove member name from stack
            checkpointAgent->popNameElement();
    
        } else {
            std::cerr << "Found a bitfield member named " << member->getName() << " - skipping for now" << std::endl;
        }
    }
}

void CheckpointVisitor::visitArrayType(const ArrayDataType * node) {
    std::cout << "Visiting ArrayDataType with subtype " << node->getTypeSpecName() << std::endl;

    // std::cout << "Elements in array: " << 
    const DataType * subtype = node->getSubType();
    for (int i = 0; i < node->getElementCount(); i++) {
        std::cout << "Going to element " << i << std::endl;

        // Push element onto stack
        checkpointAgent->pushIndex(i);

        subtype->accept(this);

        // Remove element from stack
        checkpointAgent->popNameElement();
    }

}

void CheckpointVisitor::visitPointerType(const PointerDataType * node) {
    std::cout << "Visiting PointerDataType named " << node->toString() << std::endl;
}

void CheckpointVisitor::visitEnumeratedType(const EnumDataType * node) {
    std::cout << "Visiting EnumDataType named " << node->toString() << std::endl;
}