#pragma once

#include <vector>
#include <stdexcept>

#include "Type/BaseType.hpp"
#include "Value/Value.hpp"

#include "Enumerator.hpp"
#include "Type/EnumDictionary.hpp"



/**
 An EnumDataType represents an enumeration type.
*/
class EnumDataType : public BaseType {

public:

    /**
     Constructor for EnumDataType.
     @param sizeof_element Size, in bytes, of one eneumeration element.
     */
    EnumDataType( EnumDictionary* enumDictionary,
                  std::string name,
                  size_t sizeof_element);

    /* ==================================================================== */
    /*                         RULE OF THREE INTERFACE                      */
    /* ==================================================================== */

    /**
     Copy Constructor.
     @param original The instance of EnumDataType that is to be copied.
     */
    EnumDataType ( const EnumDataType & original );

    /**
     Destructor for EnumDataType.
      */
    ~EnumDataType ();

    /**
     Assignment operator for EnumDataType.
     @param rhs right-hand-side.
    */
    EnumDataType & operator=( const EnumDataType & rhs );

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */

    /**
     */
    DataType * clone() const override;

    /**
     */
    bool validate() override;

    /**
     */
    TypeClass::e getTypeClass() const override{
        return TypeClass::ENUMERATION;
    }

    /**
     @return The size (in bytes) of an instance of the EnumDataType.
     */
    size_t getSize() const override;

    /**
     */
    void* createInstance(unsigned int num) const override;

    /**
     */
    void deleteInstance(void* address) const override;

    /**
     */
    void clearValue(void * address) const override;

    /**
     Assign a value to the (unarrayed) element at the given base-address and
     offset, where the offset is in the final dimension.
     @param address Base-address of the (possibly arrayed) variable.
     @param offset Offset, in the final dimension, to the particular element
     being assigned.
     @param value The Value to be assigned to the element.
     */
    void assignValue(void * address, Value * value) const override;

    /**
     Print the value of the element at the given base-address and offset, to the
     given stream.
     @param s The stream to print to.
     @param base_addr Address of the (entire) variable.
     @param offset Offset to the element, in the final dimension of the variable.
     */
    void printValue(std::ostream &s, void *base_addr) const override;

    /**
     */
    std::string toString() const override;

    /**
     Get the name of the enumeration type.
     */
    std::string getTypeSpecName() const override;

    bool accept (DataTypeVisitor* visitor) const override;


    /* ==================================================================== */
    /*                       CLASS SPECIFIC INTERFACE                       */
    /* ==================================================================== */

    /**
     Add a regular, non-bitfield data member to the EnumDataType.
     @param member_name Name of the data member.
     @param offset The offset (in bytes) of the data-member from the beginning of
     the struct, union or class.
     @param type_decl TypeDeclaration of the data-member.
     */
    void addEnumerator( std::string member_name, int value)  ;

private:
    EnumDataType() {};
    std::vector<Enumerator*> enum_list;
    size_t enumSize;
    EnumDictionary * enumDictionary;
    std::string name;
};
