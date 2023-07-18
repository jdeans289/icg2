#include <iostream>
#include <sstream>

#include "Type/EnumDataType.hpp"
#include "Value/NumericValue.hpp"
#include "Value/IntegerValue.hpp"
#include "Type/EnumDictionary.hpp"

const std::string EnumDataType::invalid_str("<INVALID>");

EnumDataType::EnumDataType( EnumDictionary * dict, std::string n, size_t s) : enumDictionary(dict), name(n) {
    if (!(s == sizeof(int) || s == sizeof(short) || s == sizeof(char))) {
        // Ensure that the size will always be valid so we don't have to check for this every time.
        throw std::logic_error("Cannot support enum of size " + std::to_string(s));
    }

    enumSize = s;
}

/* ================================================================================= */
/*                         RULE OF THREE (and a half) INTERFACE                      */
/* ================================================================================= */

EnumDataType::EnumDataType ( const EnumDataType & original ) : EnumDataType(original.enumDictionary, original.name, original.enumSize) {
    enumDictionary = original.enumDictionary;
    name = original.name;
    enumSize = original.enumSize;

    for ( auto enumerator : original.enum_list ) {
        enum_list.push_back( new Enumerator( *enumerator ) );
    }
}

EnumDataType::~EnumDataType () {
    for ( auto enumerator :  enum_list ) {
        delete enumerator;
    }
}

EnumDataType & EnumDataType::operator=( EnumDataType rhs ) {
    swap(*this, rhs);
    return *this;
}

void swap (EnumDataType& a, EnumDataType& b) {
    // enable ADL
    using std::swap;

    swap(a.enumDictionary, b.enumDictionary);
    swap(a.name, b.name);
    swap(a.enumSize, b.enumSize);
    swap(a.enum_list, b.enum_list);
}


/* ==================================================================== */
/*                           VIRTUAL INTERFACE                          */
/* ==================================================================== */

DataType * EnumDataType::clone () const {
    return new EnumDataType( *this );
}

bool EnumDataType::validate() {
    return true;
}

bool EnumDataType::isValid() const {
    return true;
}

size_t EnumDataType::getSize() const {
    return enumSize;
}

void* EnumDataType::createInstance(unsigned int num) const {
    void* temp = (void*)calloc(num, enumSize);
    return temp;
}

void EnumDataType::deleteInstance(void* address) const {
    free(address);
}

void EnumDataType::clearValue(void * address) const {
    if (enumSize == sizeof(int)) {
       *(int*)address = 0;
    } else if (enumSize == sizeof(short)) {
       *(short*)address = 0;
    } else {
       *(char*)address = 0;
    }
}

bool EnumDataType::assignValue(void * address, int value) const {

    if (!containsValue(value)) {
        // We're trying to assign a value that is not part of this enum.
        return false;
    }

    if (enumSize == sizeof(int)) {
        *(int*)address = (int) value;
    } else if (enumSize == sizeof(short)) {
        *(short*)address = (short) value;
    } else {
        *(char*)address = (char) value;
    } 

    return true;

}


int EnumDataType::getValue(void *address) const {
    int val;

    if (enumSize == sizeof(int)) {
        val = *(int*) address;
    } else if (enumSize == sizeof(short)) {
        val = *(short*)address;
    } else {
        val = *(char*)address;
    }

    return val;
}

// MEMBER FUNCTION
std::string EnumDataType::toString() const {
    std::stringstream ss;
    int enum_count = enum_list.size() ;
    ss << "enum {\n";
    for (int ii=0; ii < enum_count ; ii++) {
        if (ii) {
            ss << ",\n";
        }
        ss << enum_list[ii]->toString();
    }
    ss << "}\n";
    return ss.str();
}

// MEMBER FUNCTION
std::string EnumDataType::getTypeSpecName() const {
    return name;
}

bool EnumDataType::accept (DataTypeVisitor * visitor) const {
    return visitor->visitEnumeratedType(this);
}

void EnumDataType::addEnumerator( std::string name, int value)  {

    Enumerator* enumerator = new Enumerator( name, value );
    enum_list.push_back(enumerator);

    enumDictionary->addEnumerator(name, value);
}

std::string EnumDataType::lookupEnumeratorName(int value) const {
    for (auto enumerator : enum_list) {
        if (value == enumerator->getValue()) {
            return enumerator->getName();
        }
    }

    return invalid_str;
}

bool EnumDataType::containsValue(int value) const {
    for (auto enumerator : enum_list) {
        if (value == enumerator->getValue()) {
            return true;
        }
    }

    return false;
}



