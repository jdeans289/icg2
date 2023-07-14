#pragma once

#include <sstream>
#include <iostream>

#include "Type/StructMember.hpp"
#include "Value/IntegerValue.hpp"



/**
  BitfieldStructMember represents a bitfield data-member of a CompositeDeclaration.
 */
template <class T>
class BitfieldStructMember : public StructMember {

    public:

    /**
     Constructor for BitfieldStructMember.
     @param name Name of the data-member.
     @param getter Pointer to function that gets the bitfield value.
     @param setter Pointer to function that gets the bitfield value.
     */
    BitfieldStructMember( std::string name,
                          T(*getter)(void* address),
                          void(*setter)(void* address, T value))
        : StructMember(name) {

        get_bitfield_value = getter;
        set_bitfield_value = setter;
    }

    /**
    Copy Constructor.
    */
    BitfieldStructMember ( const BitfieldStructMember<T> & original) : StructMember( original ) {

        get_bitfield_value = original.get_bitfield_value;
        set_bitfield_value = original.set_bitfield_value;
    }

    /**
    Clone.
    */
    StructMember * clone () const {
        return new BitfieldStructMember( *this );
    }

    /**
    */
    bool validate() { return true; }

    /**
    */
    MemberClass::e getMemberClass() const {
        return MemberClass::BITFIELD;
    }

    /**
    Operator=
    */
    BitfieldStructMember<T> & operator=(const BitfieldStructMember<T> & rhs) = delete;

    /**
    Destructor
     */
    ~BitfieldStructMember() { /* Nothing to do */ }

    /**
     */
    void clearValue(void *struct_address) const {
        (*set_bitfield_value)(struct_address, 0);
    }

    /**
     */
    void assignValue(void *struct_address,  Value *v) const {
        IntegerValue * integer_value_p =  dynamic_cast<IntegerValue*>(v);
        if (integer_value_p) {
            T rvalue = integer_value_p->getIntegerValue();

            (*set_bitfield_value)( struct_address, rvalue );

        } else {
            std::cerr << "ERROR: Attempt to assign non-integer value to a bitfield.";
        }
    }

    Value * getValue(void *struct_address) const {
        // TODO: TEST ME
        return new IntegerValue((*get_bitfield_value)( struct_address ));
    }

    /**
     */
    void printValue(std::ostream &s, void *struct_address) const {

        T value = (*get_bitfield_value)( struct_address );
        s << value ;
    }

    // void checkpointValue(std::ostream &s, std::string varName, void *struct_address) const {
    //     std::string full_member_name = varName + "." + getName();
    //     s << full_member_name << " = ";
    //     printValue(s, struct_address);
    //     s << ";" << std::endl;
    // }

    std::string getTypeSpecName() const {
        std::string s("<invalid-bitfield-type>");
        return s;
    }

    /**
     */
    std::string toString() const {
        std::ostringstream oss;
        oss << getTypeSpecName() << " " << getName() << " : BITFIELD";
        return oss.str();
    }

    private:
    T (* get_bitfield_value)(void* addr);
    void (* set_bitfield_value)(void* addr, T value);
};
