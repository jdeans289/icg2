#pragma once

#include <vector>
#include <stdexcept>

#include "Type/BaseType.hpp"

#include "Enumerator.hpp"
#include "Type/EnumDictionary.hpp"



/**
 * @brief Enum represents an Enumerated data type.
 * @note since the underlying type can vary in size, it would possibly be cleaner to implement this in a similar way to the Primitive/SpecifiedPrimititive<T>
 * hierarchy. However, I think since there are only 3 possible sizes, maybe this is ok.
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
     * @brief Set the instance of this type at the given address to 0.
     * 
     * @param address address of instance
     */
    void clearValue(void * address) const;

    /**
     * @brief Assign the value to the instance of this data member 
     * 
     * @param address Address to assign to
     * @param value integer value to assign. Must be a pre-defined value for this type.
     */
    bool assignValue(void * address, int value) const;

    /**
     * @brief Get the Value of an instance of this type with the given address
     * 
     * @param address Address to pull the value from
     * @return int numerical value of the enum instance
     */
    int getValue(void *address) const;

    /**
     @brief Add a name-value pair to this enum data type.
     @param val_name literal name of this enum member
     @param value integer value associated
     */
    void addEnumerator( std::string val_name, int value);

    /**
     * @brief Get the string name that corresponds to the integer for this type
     * 
     * @param value integer value to find
     * @return std::string name of this value for this type, or empty string if value is not named in this enum
     */
    std::string lookupEnumeratorName(int value) const;

        /**
     * @brief Determine if this enum type has something defined to this value
     * 
     * @param value integer value to find
     * @return true if found, false otherwise
     */
    bool containsValue(int value) const;

    static const std::string invalid_str;

private:
    EnumDataType() {};
    std::vector<Enumerator*> enum_list;
    size_t enumSize;
    std::string name;

    // Pointer to EnumDictionary that this belongs to
    // TODO: does this belong here?
    EnumDictionary * enumDictionary;
};


