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

    /* ================================================================================= */
    /*                         RULE OF THREE (and a half) INTERFACE                      */
    /* ================================================================================= */

    EnumDataType ( const EnumDataType & original );
    ~EnumDataType ();
    EnumDataType & operator=( EnumDataType rhs ) ;

    friend void swap (EnumDataType& a, EnumDataType& b) ;

    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                         */
    /* ==================================================================== */

    /**
     */
    DataType * clone() const override;

    /**
     */
    bool validate() override;

    bool isValid() const override;

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

    void assignValue(void * address, Value * value) const override;

    Value * getValue(void *address) const override;

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
    void addEnumerator( std::string member_name, int value);

    /**
     * @brief Get the string name that corresponds to the integer for this type
     * 
     * @param value integer value to find
     * @return std::string name of this value for this type, or empty string if value is not named in this enum
     */
    std::string lookupEnumeratorName(int value) const;

private:
    EnumDataType() {};
    std::vector<Enumerator*> enum_list;
    size_t enumSize;
    std::string name;

    // Pointer to EnumDictionary that this belongs to
    // TODO: does this belong here?
    EnumDictionary * enumDictionary;
};


