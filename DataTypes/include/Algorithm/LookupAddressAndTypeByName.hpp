#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include "Value/Value.hpp"
#include "Utils/MutableVariableName.hpp"

// More accurate name would be
//    LookupAddressAndTypeByNameVisitor
// but that's so long

namespace LookupAddressAndTypeByName {

    struct Result {
        Result() : success(false), type(NULL), address(NULL), isPointer(false) {}

        bool success;
        const DataType * type;
        void * address;
        bool isPointer;
    };

    class LookupAddressAndTypeByNameVisitor : public DataTypeVisitor {

        public:
            LookupAddressAndTypeByNameVisitor(void * starting_address, std::string full_variable_name);
            LookupAddressAndTypeByNameVisitor(void * starting_address, MutableVariableName name_elems);

            // Visitor Interface 
            virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) override;
            virtual bool visitCompositeType(const CompositeDataType * node) override;
            virtual bool visitArrayType(const ArrayDataType * node) override;
            virtual bool visitPointerType(const PointerDataType * node) override;
            virtual bool visitEnumeratedType(const EnumDataType * node) override;
            virtual bool visitStringType (const StringDataType * node) override;
            virtual bool visitSequenceType (const SequenceDataType * node) override;

            Result getResult();

        private:
            // Helper
            bool visitLeaf(const DataType * node);

            // Visitor Intermediate State
            void * current_search_address;
            MutableVariableName name_elems;

            // Result 
            Result result;
    };
}