#pragma once

#include "Algorithm/DataTypeVisitor.hpp"
// #include "CheckpointAgent.hpp"
#include "Value/Value.hpp"
#include "Utils/MutableVariableName.hpp"

#include <vector>
#include <stack>



class CheckpointVisitor : public DataTypeVisitor {

    public:
        CheckpointVisitor();
        CheckpointVisitor(std::string starting_name, void * starting_address);

        // Visitor Interface 

        virtual bool visitPrimitiveDataType(const PrimitiveDataType * node) override;
        virtual bool visitCompositeType(const CompositeDataType * node) override;
        virtual bool visitArrayType(const ArrayDataType * node) override;
        virtual bool visitPointerType(const PointerDataType * node) override;
        virtual bool visitEnumeratedType(const EnumDataType * node) override;
        virtual bool visitStringType (const StringDataType * node) override;

        // Checkpoint Specific Interface

        // Structure for returning results
        struct Leaf {
            Leaf(MutableVariableName n, Value * v) : Leaf(n, v, false, NULL) {}
            Leaf(MutableVariableName n, Value * v, bool is_ptr, const DataType * ptr_sub) : name_stack(n), value(v), is_pointer(is_ptr), pointer_subtype(ptr_sub) {}


            // For debugging:
            std::string toString() {
                std::stringstream ss;
                ss << "Name: " << name_stack.toString() << "\tValue: " << value->toString();
                return ss.str();
            }

            MutableVariableName name_stack;
            Value * value;
            bool is_pointer;
            const DataType * pointer_subtype;
        };

        std::vector <Leaf> getResults();

    private:
        // Visitor State
        MutableVariableName current_name_stack;
        std::stack<void *> address_stack;

        std::vector<Leaf> leaves;
};