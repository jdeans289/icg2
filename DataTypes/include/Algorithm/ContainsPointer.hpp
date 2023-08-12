#pragma once

#include "Algorithm/DataTypeVisitor.hpp"


namespace ContainsPointer {

    typedef bool Result;

    class ContainsPointerVisitor : public DataTypeVisitor {

        public:
            ContainsPointerVisitor();

            // Visitor Interface 
            virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) override;
            virtual bool visitCompositeType(const CompositeDataType * node) override;
            virtual bool visitArrayType(const ArrayDataType * node) override;
            virtual bool visitPointerType(const PointerDataType * node) override;
            virtual bool visitEnumeratedType(const EnumDataType * node) override;
            virtual bool visitStringType (const StringDataType * node) override;
            virtual bool visitSequenceType (const SequenceDataType * node) override;

    };
}