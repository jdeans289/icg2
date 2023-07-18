#include <algorithm>

#include "Algorithm/PrintValue.hpp"
#include "Type/Types.hpp"
#include "Type/NormalStructMember.hpp"

namespace PrintValue {

    PrintValueVisitor::PrintValueVisitor(std::ostream &s, void *address) : s(s) {
        address_stack.push(address);
    }

    bool PrintValueVisitor::visitPrimitiveDataType(const PrimitiveDataType * node) {
        node->printValue(s, address_stack.top());
        return true;
    }

    bool PrintValueVisitor::visitCompositeType(const CompositeDataType * node) {
        // MEMBER FUNCTION
        s << "{";
        int counter = 0;

        // Static members

        // Normal members
        for (auto it = node->getNormalMemberListBegin(); it != node->getNormalMemberListEnd(); it++, counter++) {
            if (counter != 0) {
                s << ", ";
            }
            
            NormalStructMember * member = *it;
            const DataType * member_subtype = member->getSubType();
            address_stack.push(member->getAddress(address_stack.top()));

            // Go into member
            member_subtype->accept(this);

            // Remove member name from stack
            address_stack.pop();
        }
        s << "}";

        return true;
    }

    bool PrintValueVisitor::visitArrayType(const ArrayDataType * node) {
        s << "{";

        for (unsigned int i=0; i < node->getElementCount() ; i++) {
            if (i) {
                s << ",";
            }

            const DataType * subType = node->getSubType();

            address_stack.push( (char*) address_stack.top() + (i * subType->getSize()) );
            subType->accept( this );
            address_stack.pop();
        }
        s << "}";

        return true;
    }

    bool PrintValueVisitor::visitPointerType(const PointerDataType * node) {
        s << *(void**)address_stack.top();

        return true;
    }

    bool PrintValueVisitor::visitEnumeratedType(const EnumDataType * node) {
        int enum_val = *(int*) address_stack.top();
        s << node->lookupEnumeratorName(enum_val);

        return true;
    }

    bool PrintValueVisitor::visitStringType (const StringDataType * node) {
        s << "\"" <<  * (std::string * ) address_stack.top() << "\"";

        return true;
    }


}