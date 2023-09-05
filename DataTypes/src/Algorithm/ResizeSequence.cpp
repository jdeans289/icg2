#include <cassert>

#include "Algorithm/ResizeSequence.hpp"

#include "Type/VisitableTypes.hpp"

namespace ResizeSequence {

    ResizeSequenceVisitor::ResizeSequenceVisitor(void * starting_address, int num_elems) 
            : address(starting_address),  num_elems(num_elems) {}

    // Target type!
    bool ResizeSequenceVisitor::visitSequenceType (std::shared_ptr<const SequenceDataType>  node) {
        return node->resize(address, num_elems);
    }

    // Everything else is wrong

    bool ResizeSequenceVisitor::visitCompositeType(std::shared_ptr<const CompositeDataType> node) {
        return visitIncorrectType(node);
    }

    bool ResizeSequenceVisitor::visitArrayType(std::shared_ptr<const ArrayDataType> node) {
        return visitIncorrectType(node);
    }

    bool ResizeSequenceVisitor::visitPointerType(std::shared_ptr<const PointerDataType> node) {
        return visitIncorrectType(node);
    }

    bool ResizeSequenceVisitor::visitPrimitiveDataType(std::shared_ptr<const PrimitiveDataType> node) {
        return visitIncorrectType(node);
    }

    bool ResizeSequenceVisitor::visitEnumeratedType(std::shared_ptr<const EnumDataType> node) {
        return visitIncorrectType(node);
    }

    bool ResizeSequenceVisitor::visitStringType(std::shared_ptr<const StringDataType> node) {
        return visitIncorrectType(node);
    }

    bool ResizeSequenceVisitor::visitIncorrectType(std::shared_ptr<const DataType> node) {
        // We're at a leaf. We should just not be here at all.
        std::cerr << "Tried to use ResizeSequence on something that is not a sequence." << std::endl;

        return false;
    }
}