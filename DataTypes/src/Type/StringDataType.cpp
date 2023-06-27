#include <iostream>
#include <sstream>

#include "Type/StringDataType.hh"
#include "Value/StringValue.hh"


StringDataType::StringDataType(std::string typeSpecifierName) {

    // this->dataTypeInator = dataTypeInator;
    this->typeSpecName = typeSpecifierName;
}

/* ==================================================================== */
/*                         RULE OF THREE INTERFACE                      */
/* ==================================================================== */

StringDataType::StringDataType ( const StringDataType & original ) {

}

StringDataType::~StringDataType () {

}

StringDataType & StringDataType::operator=( const StringDataType & rhs ) {
   return *this;
}

/* ==================================================================== */
/*                           VIRTUAL INTERFACE                          */
/* ==================================================================== */

DataType * StringDataType::clone () const {
    return new StringDataType( *this );
}

bool StringDataType::validate() {
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

void StringDataType::clearValue(void * address) const {
   ((std::string * ) address)->clear();
}

void StringDataType::assignValue(void * address, Value * value) const {
    StringValue * str_value = dynamic_cast <StringValue * > (value);

    if (str_value == NULL) {
        std::cerr << "ERROR: Attempt to assign non-string value to a string type.";
        return;
    }

    * (std::string * ) address = str_value->getRawString();
}

void StringDataType::printValue(std::ostream &s, void *address) const {
    s << * (std::string * ) address;
}

Value * StringDataType::getValue(void *address) const {
    return new StringValue(* (std::string * ) address);
}

// MEMBER FUNCTION
std::string StringDataType::getTypeSpecName() const {
    return typeSpecName;
}

bool StringDataType::accept (DataTypeVisitor* visitor) const {
    return visitor->visitStringType(this);
}

