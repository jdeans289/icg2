#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
#include "Value/Value.hpp"
#include "Utils/MutableVariableName.hpp"

#include <vector>
#include <stack>


namespace FindLeaves {

    // Struct for returning results
    struct Leaf {
        Leaf(MutableVariableName n, Value * v) : Leaf(n, v, false, NULL) {}
        Leaf(MutableVariableName n, Value * v, bool is_ptr, const DataType * ptr_sub) : name_stack(n), value(v), is_pointer(is_ptr), pointer_subtype(ptr_sub) {}

        MutableVariableName name_stack;
        Value * value;
        bool is_pointer;
        const DataType * pointer_subtype;
    };

    typedef std::vector<Leaf> Result;
            
    class FindLeavesVisitor : public DataTypeVisitor {

        public:
            FindLeavesVisitor();
            FindLeavesVisitor(std::string starting_name, void * starting_address);

            // Visitor Interface 

            virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) override;
            virtual bool visitCompositeType(const CompositeDataType * node) override;
            virtual bool visitArrayType(const ArrayDataType * node) override;
            virtual bool visitPointerType(const PointerDataType * node) override;
            virtual bool visitEnumeratedType(const EnumDataType * node) override;
            virtual bool visitStringType (const StringDataType * node) override;

            // FindLeavesVisitor Specific Interface
            Result getResult();

        private:
            // Visitor State
            MutableVariableName current_name_stack;
            std::stack<void *> address_stack;

            // Result
            std::vector<Leaf> leaves;
    };

}
