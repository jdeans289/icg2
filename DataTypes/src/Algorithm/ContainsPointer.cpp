#include "Algorithm/ContainsPointer.hpp"

#include "Type/Types.hpp"
#include "Type/TypedStructMember.hpp"

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

        for (int i = 0; i < node->getMemberCount(); i++) {
            StructMember * member = node->getStructMember(i);

            // ugh
            // We should figure out how to get rid of this
            // The bitfields really do bad things to the design
            TypedStructMember * typed_member = dynamic_cast<TypedStructMember *> (member);

            if (typed_member != NULL) {
                const DataType * member_subtype = typed_member->getDataType();
                
                // Go into member
                result |= member_subtype->accept(this);
            } 
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

}