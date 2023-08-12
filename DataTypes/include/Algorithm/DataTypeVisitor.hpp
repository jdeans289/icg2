#pragma once

// Abstract class for a DataTypeVisitor

class DataType;
class CompositeDataType;
class ArrayDataType;
class PointerDataType;
class EnumDataType;
class StringDataType;
class PrimitiveDataType;
class SequenceDataType;

/**
 * @brief Abstract base class for a DataTypeVisitor
 * 
 */
class DataTypeVisitor {
    public:
        virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) = 0;
        virtual bool visitEnumeratedType(const EnumDataType * node) = 0;
        virtual bool visitCompositeType(const CompositeDataType * node) = 0;
        virtual bool visitArrayType(const ArrayDataType * node) = 0;
        virtual bool visitPointerType(const PointerDataType * node) = 0;
        virtual bool visitStringType (const StringDataType * node) = 0;
        virtual bool visitSequenceType(const SequenceDataType * node) = 0;

        virtual bool go(const DataType * node);
};