#pragma once

#include "Algorithm/DataTypeVisitor.hh"
#include "Value/Value.hh"
#include "Utils/VariableNameStack.hh"


class LookupAddressVisitor : public DataTypeVisitor {

    public:
        LookupAddressVisitor(std::string starting_name, void * starting_address, void * lookup_address, const DataType * const search_type);
        LookupAddressVisitor(std::string starting_name, void * starting_address, void * lookup_address);

        // Visitor Interface 

        virtual bool visitPrimitiveDataType(const DataType * node) override;

        virtual bool visitCompositeType(const CompositeDataType * node) override;
        virtual bool visitArrayType(const ArrayDataType * node) override;
        virtual bool visitPointerType(const PointerDataType * node) override;
        virtual bool visitEnumeratedType(const EnumDataType * node) override;

        // LookupAddress Interface
        VariableNameStack getResult();


    private:
        // Visitor State
        
        // We don't actually care about the absolute address, only the offset within the type
        // Mutable address to read only value
        long search_offset;

        // Const all around
        const DataType * const search_type;

        VariableNameStack name_stack;

        // Helper
        bool typeCheck(const DataType * node);
};