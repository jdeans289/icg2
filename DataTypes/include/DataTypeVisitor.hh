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
        virtual void visitPrimitiveDataType(const DataType * node) = 0;
        virtual void visitEnumeratedType(const EnumDataType * node) = 0;

        // Enter
        virtual void visitCompositeType(const CompositeDataType * node) = 0;
        virtual void visitArrayType(const ArrayDataType * node) = 0;
        virtual void visitPointerType(const PointerDataType * node) = 0;

        // Leave
        // virtual void leaveCompositeType(const CompositeDataType * node) = 0;
        // virtual void leaveArrayType(const ArrayDataType * node) = 0;
        // virtual void leavePointerType(const PointerDataType * node) = 0;
};