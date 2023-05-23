#pragma once

#include "DataType.hh"

class ModifierType : public DataType {

    public: 
        const DataType * getSubType() const {
            return subType;
        }

    protected:
        const DataType * subType;
};