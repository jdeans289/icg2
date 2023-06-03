#pragma once

#include "DataTypeVisitor.hh"
#include "CheckpointAgent.hh"

class CheckpointVisitor : public DataTypeVisitor {

    public:
        CheckpointVisitor(CheckpointAgent * agent);

        virtual void visitPrimitiveDataType(const DataType * node);

        virtual void visitCompositeType(const CompositeDataType * node);
        virtual void visitArrayType(const ArrayDataType * node);
        virtual void visitPointerType(const PointerDataType * node);
        virtual void visitEnumeratedType(const EnumDataType * node);

    private:
        CheckpointAgent * checkpointAgent;
};