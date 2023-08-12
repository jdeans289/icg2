#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include "Value/Value.hpp"
#include "Utils/MutableVariableName.hpp"

#include <vector>
#include <stack>


namespace ResizeSequence {
            
    class ResizeSequenceVisitor : public DataTypeVisitor {

        public:
            ResizeSequenceVisitor(void * address, std::string variable_name, int num_elems);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) override;
            virtual bool visitCompositeType(const CompositeDataType * node) override;
            virtual bool visitArrayType(const ArrayDataType * node) override;
            virtual bool visitPointerType(const PointerDataType * node) override;
            virtual bool visitEnumeratedType(const EnumDataType * node) override;
            virtual bool visitStringType (const StringDataType * node) override;
            virtual bool visitSequenceType (const SequenceDataType * node) override;

        private:

            bool visitLeaf(const DataType * node);

            // Visitor Intermediate State
            void * current_search_address;
            MutableVariableName name_elems;
            int num_elems;
    };
}
