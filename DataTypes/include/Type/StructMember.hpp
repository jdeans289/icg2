#pragma once

#include <string>

#include "DataTypeInator.hpp"

/**
 * @brief Represent a normal or static member of a struct.
 * 
 */
struct StructMember {

public:

    enum StorageClass {
        NORMAL,
        STATIC
    };

    /**
     Constructor.
     */
    StructMember( std::string memberName, std::string typeSpecName, int address, StorageClass storage = NORMAL);
    ~StructMember();

    /**
     * @brief toString
     * 
     * @return std::string 
     */
    std::string toString() const;

    /**
     * @brief Get the address of this member given the base address of a struct containing it
     * 
     * @param structAddress base address of struct - may be excluded if we're working with a static member
     * @return void* address of member
     */
    void * getAddressOfMember (void * structAddress = NULL) const;

    /**
     * @brief Enable sorting by offset
     * 
     */
    inline bool operator< (const StructMember& other) const {
        return (memberAddress < other.memberAddress);
    }


    /* ==================================================================== */
    /*                      Trivial getters/setters                         */
    /* ==================================================================== */

    /**
     * @brief Get the name of this struct member
     * 
     * @return std::string 
     */
    inline std::string getName() const { return name; }

    /**
     * @brief Get the type name of this struct member
     * 
     * @return std::string 
     */
    inline std::string getTypeSpecName() const { return typeSpecName; }

    /**
     * @brief Get the type of this member
     * 
     * @return std::shared_ptr<const DataType> 
     */
    inline std::shared_ptr<const DataType> getSubType() const { return subType; }

    /**
     * @brief Set the SubType of this member
     * 
     * @param type subtype to set
     */
    inline void setSubType(std::shared_ptr<const DataType> type) { subType = type; }

    /**
     * @brief Get the storage class of this member
     * 
     * @return StorageClass 
     */
    inline StorageClass getStorageClass () const { return storage; }

    private:
    std::string name;
    std::string typeSpecName;
    long memberAddress;
    StorageClass storage;
    std::shared_ptr<const DataType> subType;
};
