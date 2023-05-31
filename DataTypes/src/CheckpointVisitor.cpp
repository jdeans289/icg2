#include "CheckpointVisitor.hh"

#include "DataType.hh"
#include "CompositeDataType.hh"
#include "ArrayDataType.hh"
#include "PointerDataType.hh"
#include "EnumDataType.hh"



CheckpointVisitor::CheckpointVisitor() {

}

void CheckpointVisitor::visitPrimitiveDataType(const DataType * node) {
    std::cout << "Visiting PrimitiveDataType named " << node->toString() << std::endl;
}

void CheckpointVisitor::visitCompositeType(const CompositeDataType * node) {
    std::cout << "Visiting CompositeDataType named " << node->toString() << std::endl;
    
}

void CheckpointVisitor::visitArrayType(const ArrayDataType * node) {
    std::cout << "Visiting ArrayDataType named " << node->toString() << std::endl;
}

void CheckpointVisitor::visitPointerType(const PointerDataType * node) {
    std::cout << "Visiting PointerDataType named " << node->toString() << std::endl;
}

void CheckpointVisitor::visitEnumeratedType(const EnumDataType * node) {
    std::cout << "Visiting EnumDataType named " << node->toString() << std::endl;
}