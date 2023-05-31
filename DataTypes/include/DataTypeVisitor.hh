#pragma once


// Abstract class for a DataTypeVisitor


// #include "DataType.hh"
// #include "PrimitiveDataType.hh"
// #include "CompositeDataType.hh"
// #include "ArrayDataType.hh"
// #include "PointerDataType.hh"
// #include "EnumDataType.hh"

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
        virtual void visitPrimitiveDataType(const DataType * node) = 0;

        virtual void visitCompositeType(const CompositeDataType * node) = 0;
        virtual void visitArrayType(const ArrayDataType * node) = 0;
        virtual void visitPointerType(const PointerDataType * node) = 0;
        virtual void visitEnumeratedType(const EnumDataType * node) = 0;
};