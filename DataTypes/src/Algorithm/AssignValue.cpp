#include "Algorithm/AssignValue.hpp"

#include "Type/VisitableTypes.hpp"

#include "Type/NormalStructMember.hpp"
#include "Value/Value.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/PointerValue.hpp"
#include "Value/StringValue.hpp"


/**
 * @brief Assign to a leaf value.
 * @note I don't think we should support CompositeValue, ArrayValue, and other possible future multi-value containers.
 * I think we should only support "leaf" types - Primitive, String, Pointer, Enum
 * And leave it up to other algorithms to find this types
 * This is already what J_Checkpoint agent does
 * However, if we switch to another format of checkpoint, like json or xml, which would lend itself to reconstructing entire objects rather than just leaves.
 * This might be annoying if we decide to consolidate arrays or something
 * idk maybe not
 * It's simple enough to move the functionality here, it's probably better to keep the support rather than drop that.
 * Actually no. I think tying the 'Value' hierarchy directly to the 'DataTypes' hierarchy is a bad idea. I want to drop that.
 * If we want to create whole values in the future, we can make another DataTypeAlgorithm for that. But this isn't what I want to do.
 * I also really hate that we're dynamic casting all over the place
 * But I don't see a way around it without ~really~ tying the Value and Type hierarchies together
 */
namespace AssignValue {

    AssignValueVisitor::AssignValueVisitor(Value * value, void * address) : value_to_assign(value), address(address) {}

    bool AssignValueVisitor::visitPrimitiveDataType(const PrimitiveDataType * node) {
        // Primitives are actually an abstract base for the SpecifiedPrimitive<T>  type
        // So we use the assignValue() interface from PrimitiveType

        return node->assignValue(address, value_to_assign);
    }

    bool AssignValueVisitor::visitStringType(const StringDataType * node) {
        // TODO: implement double dispatch visitors for the Value hierarchy.... maybe
        StringValue * str_value = dynamic_cast <StringValue * > (value_to_assign);

        if (str_value == NULL) {
            std::cerr << "ERROR: Attempt to assign non-string value to a string type." << std::endl;
            return false;
        }

        *(std::string *) address = str_value->getRawString();
        return true;
    }

    bool AssignValueVisitor::visitCompositeType(const CompositeDataType * node) {
        std::cerr << "Must assign to leaf type" << std::endl;
        return false;
    }

    bool AssignValueVisitor::visitArrayType(const ArrayDataType * node) {
        std::cerr << "Must assign to leaf type" << std::endl;
        return false;
    }

    bool AssignValueVisitor::visitPointerType(const PointerDataType * node) {
        PointerValue * pointer_value_p = dynamic_cast<PointerValue*>(value_to_assign);
        if (pointer_value_p) {
            *(void**)address =  pointer_value_p->getPointer();
            return true;
        } else {
            std::cerr << "ERROR: Attempt to assign non-pointer value to a pointer." << std::endl;
            return false;
        }
    }

    bool AssignValueVisitor::visitEnumeratedType(const EnumDataType * node) {
        IntegerValue * e_val = dynamic_cast<IntegerValue *> (value_to_assign);

        if (e_val) {
            return node->assignValue(address, e_val->getIntegerValue());
        } else {
            std::cerr << "ERROR: Attempt to assign non-integral value to an enum." << std::endl;
            return false;
        }
    }

    bool AssignValueVisitor::visitSequenceType (const SequenceDataType * node) {
        std::cerr << "Must assign to leaf type" << std::endl;
        return false;
    }

}