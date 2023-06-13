#pragma once


// Abstract class for a DataTypeVisitor

class DataType;
class CompositeDataType;
class ArrayDataType;
class PointerDataType;
class EnumDataType;



class DataTypeVisitor {
    public:
        
        // PrimitiveDataType is a templated class
        // And you can't have a virtual templated function
        // So we just take a DataType
        // This is kinda bad, but I can't think of a way around it
        
        // Primitive, Enum Type is a leaf
        virtual bool visitPrimitiveDataType(const DataType * node) = 0;
        virtual bool visitEnumeratedType(const EnumDataType * node) = 0;
        virtual bool visitCompositeType(const CompositeDataType * node) = 0;
        virtual bool visitArrayType(const ArrayDataType * node) = 0;
        virtual bool visitPointerType(const PointerDataType * node) = 0;
};