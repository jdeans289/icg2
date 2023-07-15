#include <iostream>
#include <sstream>

#include "Type/EnumDataType.hpp"
#include "Value/NumericValue.hpp"
#include "Value/IntegerValue.hpp"
#include "Type/EnumDictionary.hpp"


EnumDataType::EnumDataType( EnumDictionary * dict, std::string n, size_t s) : enumDictionary(dict), name(n) {
    if (!(s == sizeof(int) || s == sizeof(short) || s == sizeof(char))) {
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

void EnumDataType::assignValue(void * address, Value * value) const {

    NumericValue * numeric_value_p = dynamic_cast<NumericValue*>(value);
    if (numeric_value_p) {

       if (enumSize == sizeof(int)) {
           *(int*)address =  numeric_value_p->getIntegerValue();
       } else if (enumSize == sizeof(short)) {
           *(short*)address =  numeric_value_p->getIntegerValue();
       } else {
           *(char*)address =  numeric_value_p->getIntegerValue();
       } 
    } else {
        std::cerr << "ERROR: Attempt to assign non-numeric value to a numeric type.";
    }
}

Value * EnumDataType::getValue(void *address) const {
    int val;

    if (enumSize == sizeof(int)) {
        val = *(int*) address;
    } else if (enumSize == sizeof(short)) {
        val = *(short*)address;
    } else {
        val = *(char*)address;
    }

    return new IntegerValue(val);
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

    return "";
}


