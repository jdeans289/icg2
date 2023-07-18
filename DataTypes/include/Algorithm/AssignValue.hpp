#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include "Value/Value.hpp"
#include "Utils/MutableVariableName.hpp"

#include <vector>
#include <stack>


namespace AssignValue {

    typedef bool Result;
            
    class AssignValueVisitor : public DataTypeVisitor {

        public:
            AssignValueVisitor(Value * value, void * address);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) override;
            virtual bool visitCompositeType(const CompositeDataType * node) override;
            virtual bool visitArrayType(const ArrayDataType * node) override;
            virtual bool visitPointerType(const PointerDataType * node) override;
            virtual bool visitEnumeratedType(const EnumDataType * node) override;
            virtual bool visitStringType (const StringDataType * node) override;

        private:
            // Visitor State
            Value * value_to_assign;
            void * address;
    };
}
