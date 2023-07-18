#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include <stack>


namespace ClearValue {

    typedef bool Result;
            
    class ClearValueVisitor : public DataTypeVisitor {

        public:
            ClearValueVisitor(void * address);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) override;
            virtual bool visitCompositeType(const CompositeDataType * node) override;
            virtual bool visitArrayType(const ArrayDataType * node) override;
            virtual bool visitPointerType(const PointerDataType * node) override;
            virtual bool visitEnumeratedType(const EnumDataType * node) override;
            virtual bool visitStringType (const StringDataType * node) override;

        private:
            // Visitor State
            std::stack<void *> address_stack;
    };
}
