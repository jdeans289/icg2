#pragma once

#include "Type/DataType.hpp"

class ModifierType : public DataType {

    public: 
        const DataType * getSubType() const {
            return subType;
        }

    protected:
        const DataType * subType;
};