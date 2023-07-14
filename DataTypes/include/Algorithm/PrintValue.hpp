#include <iostream>
#include <stack>

#include "Algorithm/DataTypeVisitor.hpp"

namespace PrintValue {

    class PrintValueVisitor : public DataTypeVisitor {

        public:
            PrintValueVisitor(std::ostream &s, void *address);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) override;
            virtual bool visitCompositeType(const CompositeDataType * node) override;
            virtual bool visitArrayType(const ArrayDataType * node) override;
            virtual bool visitPointerType(const PointerDataType * node) override;
            virtual bool visitEnumeratedType(const EnumDataType * node) override;
            virtual bool visitStringType (const StringDataType * node) override;

        private:
            // Visitor State
            std::ostream& s;
            std::stack<void *> address_stack;
    };
}