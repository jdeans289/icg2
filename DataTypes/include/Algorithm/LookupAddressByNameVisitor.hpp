#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include "Value/Value.hpp"
#include "Utils/MutableVariableName.hpp"

// More accurate name would be
//    LookupAddressAndTypeByNameVisitor
// but that's so long

class LookupAddressByNameVisitor : public DataTypeVisitor {

    public:
        LookupAddressByNameVisitor(void * starting_address, std::string full_variable_name);
        LookupAddressByNameVisitor(void * starting_address, MutableVariableName name_elems);


        // Visitor Interface 

        virtual bool visitPrimitiveDataType(const DataType * node) override;

        virtual bool visitCompositeType(const CompositeDataType * node) override;
        virtual bool visitArrayType(const ArrayDataType * node) override;
        virtual bool visitPointerType(const PointerDataType * node) override;
        virtual bool visitEnumeratedType(const EnumDataType * node) override;
        virtual bool visitStringType (const StringDataType * node) override;

        struct Result {
            const DataType * type;
            void * address;
        };

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