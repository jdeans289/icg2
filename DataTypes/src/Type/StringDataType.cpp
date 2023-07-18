#include <iostream>
#include <sstream>

#include "Type/StringDataType.hpp"
#include "Value/StringValue.hpp"


StringDataType::StringDataType() {}

/* ==================================================================== */
/*                           VIRTUAL INTERFACE                          */
/* ==================================================================== */

DataType * StringDataType::clone () const {
    return new StringDataType( *this );
}

bool StringDataType::validate() {
    return true;
}

bool StringDataType::isValid() const {
    return true;
}

size_t StringDataType::getSize() const {
    return sizeof (std::string);
}

void* StringDataType::createInstance(unsigned int num) const {
    void* temp = (void*)calloc(num, sizeof(std::string));
    return temp;
}

void StringDataType::deleteInstance(void* address) const {
    free(address);
}

// void StringDataType::clearValue(void * address) const {
//    ((std::string * ) address)->clear();
// }


// MEMBER FUNCTION
std::string StringDataType::getTypeSpecName() const {
    return "std::string";
}

bool StringDataType::accept (DataTypeVisitor* visitor) const {
    return visitor->visitStringType(this);
}

