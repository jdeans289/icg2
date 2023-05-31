#pragma once

#include "DataTypeVisitor.hh"

class CheckpointVisitor : public DataTypeVisitor {

    public:
        CheckpointVisitor();

        virtual void visitPrimitiveDataType(const DataType * node);

        virtual void visitCompositeType(const CompositeDataType * node);
        virtual void visitArrayType(const ArrayDataType * node);
        virtual void visitPointerType(const PointerDataType * node);
        virtual void visitEnumeratedType(const EnumDataType * node);

};