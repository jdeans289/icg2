#include "Algorithm/ContainsPointer.hpp"

#include "Type/VisitableTypes.hpp"
#include "Type/NormalStructMember.hpp"

namespace ContainsPointer {

    ContainsPointerVisitor::ContainsPointerVisitor() {}

    bool ContainsPointerVisitor::visitPrimitiveDataType(const PrimitiveDataType * node) {
        return false;
    }

    bool ContainsPointerVisitor::visitStringType(const StringDataType * node) {
        return false;
    }

    bool ContainsPointerVisitor::visitCompositeType(const CompositeDataType * node) {
        bool result = false;

        for (auto it = node->getNormalMemberListBegin(); it != node->getNormalMemberListEnd(); it++) {
            NormalStructMember * member = *it;
            const DataType * member_subtype = member->getSubType();
            
            // Go into member
            result |= member_subtype->accept(this);
            
        }

        return result;
    }

    bool ContainsPointerVisitor::visitArrayType(const ArrayDataType * node) {
        const DataType * subtype = node->getSubType();
        return subtype->accept(this);
    }

    bool ContainsPointerVisitor::visitPointerType(const PointerDataType * node) {
        // This is a case that we're looking for!
        return true;
    }

    bool ContainsPointerVisitor::visitEnumeratedType(const EnumDataType * node) {
        return false;
    }

    bool ContainsPointerVisitor::visitSequenceType (const SequenceDataType * node) {
        throw std::logic_error("Sequence type not implemented in ContainsPointerVisitor");
    }

}