#pragma once

#include <string>

#include "DataTypeInator.hpp"

/**
 */
class StructMember {

public:

    /**
     Constructor.
     */
    StructMember( std::string memberName, std::string typeSpecName );

    /**
     * @brief Resolve the type
     * 
     * @return true success
     * @return false failure
     */
    bool validate(const DataTypeInator* dataTypeInator);

    /**
     * @brief Return true if this member is valid
     * 
     * @return true if the type has been resolved
     * @return false otherwise
     */
    bool isValid() const;

    /**
     * @brief toString
     * 
     * @return std::string 
     */
    virtual std::string toString() const = 0;

    /**
     * @brief Get the name of this struct
     * 
     * @return std::string 
     */
    std::string getName() const;

    /**
     * @brief Get the type of this member
     * 
     * @return const DataType* 
     */
    const DataType * getSubType() const;

private:
    bool is_valid;
    std::string name;
    std::string typeSpecName;
    const DataType * subType;
};
