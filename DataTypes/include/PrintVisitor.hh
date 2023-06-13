#pragma once

#include "DataTypeVisitor"

class PrintVisitor : DataTypeVisitor {
    public:
        void visitPrimitiveDataType(const DataType * node);
        void visitEnumeratedType(const EnumDataType * node);
        void visitCompositeType(const CompositeDataType * node);
        void visitArrayType(const ArrayDataType * node);
        void visitPointerType(const PointerDataType * node);

    private:
        // Visitor State

};