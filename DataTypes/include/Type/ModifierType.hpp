#pragma once

#include "Type/DataType.hpp"

/**
 * @brief A type that only exists to provide a modifier to a subtype. Pointer and array
 * 
 */
class ModifierType : public DataType {

    public: 
        ModifierType() : subType(NULL) {}

        const DataType * getSubType() const {
            return subType;
        }

    protected:
        const DataType * subType;
};