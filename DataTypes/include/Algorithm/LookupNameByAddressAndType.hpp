#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include "Utils/MutableVariableName.hpp"


namespace LookupNameByAddressAndType {

    struct Result {
        Result () : success(false) {}

        bool success;
        std::string name;
    };

    class LookupNameByAddressVisitor : public DataTypeVisitor {

        public:
            LookupNameByAddressVisitor(std::string starting_name, void * starting_address, void * lookup_address, const DataType * const search_type);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) override;
            virtual bool visitCompositeType(const CompositeDataType * node) override;
            virtual bool visitArrayType(const ArrayDataType * node) override;
            virtual bool visitPointerType(const PointerDataType * node) override;
            virtual bool visitEnumeratedType(const EnumDataType * node) override;
            virtual bool visitStringType (const StringDataType * node) override;

            // LookupAddress Interface
            std::string getResult();


        private:
            // Visitor State
            
            // We don't actually care about the absolute address, only the offset within the type
            long search_offset;

            // Const all around
            const DataType * const search_type;

            // Result variable
            MutableVariableName name_stack;

            // Helper
            bool typeCheck(const DataType * node);
            bool visitLeaf(const DataType * node);
    };
}