#pragma once

#include <string>
#include <vector>

#include "Value/Value.hpp"
#include "Algorithm/DataTypeVisitor.hpp"
#include "Utils/MutableVariableName.hpp"

namespace TypeClass {
    enum e {
        PRIMITIVE   = 0,
        ENUMERATION = 1,
        POINTER     = 2,
        ARRAY       = 3,
        COMPOSITE   = 4
    };
};

/** @brief Abstract base class for DataTypes. */
class DataType {

    public:

    virtual ~DataType() {}

   /**
    * @brief Verify that all of the types that are named by this DataType or
    * subordinate DataTypes are resolvable to actual DataTypes in
    * the TypeDictionary.
    * 
    * @return true if validation is successful, false otherwise 
    */
    virtual bool validate() = 0;

    /**
     * @brief Determine whether this type has already been successfully validated.
     * 
     * @return true if validated, false otherwise
     */
    virtual bool isValid() const = 0;

    /**
     * @brief The size (in bytes) of an instance of the DataType.
     * 
     * @return size_t size of this type
     */
    virtual size_t getSize() const = 0;

    /**
     * @brief Get the class of the type
     * 
     * @return TypeClass::e An enumeration value indicating which of the several specific subclasses of DataType this class is.
     * @note I kinda wanna jettison this
     */
    virtual TypeClass::e getTypeClass() const = 0;

    /**
     * @brief Get the Type Spec Name object
     * 
     * @return std::string name of the type
     */
    virtual std::string getTypeSpecName() const = 0;

    /**
     * @brief Deep copy of this type
     * 
     * @return DataType* a deep copy of this type
     */
    virtual DataType * clone () const = 0;

    /**
     * @brief Create one or more instances of this type
     * 
     * @param n number to create
     * @return void* address of the beginnning of the allocation
     */
    virtual void* createInstance(unsigned int n = 1 ) const = 0;

    /**
     * @brief Delete this instance
     * 
     * @param address of instance to delete
     * @note Is this compatible with the createInstance?
     */
    virtual void deleteInstance(void* address) const = 0;

    /**
    Clear the variable at the given address.
    @param address Address of the variable.
    */
    virtual void clearValue(void * address) const = 0;

    /**
    Assign a value to the variable at the given address.
    @param address Address of the variable.
    @param value Value to be assigned to the element.
    */
    virtual void assignValue(void * address, Value * value) const = 0;

    /**
    Creates a Value object for the variable at the given address.
    @param address Address of the variable.
    */
    virtual Value * getValue(void *address) const = 0;

    /**
    Generate a declaration of this DataType.
    @param declarator A declarator, which can be a
        1) a variable name or an array declarator (both are direct-declarators) or
        2) a pointer declarator ( not a direct-declarator ).
    */
    virtual std::string makeDeclaration(std::string declarator) const;

    /**
    Generate a definition of this DataType.
    */
    virtual std::string toString() const;

    /* 
    Implement a Visitor Pattern
    */
    virtual bool accept (DataTypeVisitor* visitor) const = 0;


    private:
};
