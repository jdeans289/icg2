#include <iostream>
#include <sstream>
#include "Type/EnumDictionary.hpp"

int EnumDictionary::getValue(std::string name )  {

    auto it = enumDictionary.find(name);
    if (it == enumDictionary.end()) {
        std::stringstream error_stream ;
        error_stream << "ERROR: Enumerator \"" << name << "\" not defined." << std::endl;
        throw std::logic_error( error_stream.str());
    } else {
        return( it->second );
    }
}

void EnumDictionary::addEnumerator(std::string name, int value)  {

    auto it = enumDictionary.find(name);
    if (it == enumDictionary.end()) {
        enumDictionary[name] = value;
    } else {
        std::stringstream error_stream ;
        error_stream << "ERROR: Attempt to re-define enumerator \"" << name << "\"." << std::endl;
        throw std::logic_error( error_stream.str());
    }
}

// MEMBER FUNCTION
std::string EnumDictionary::toString() {
    std::ostringstream oss;

    for ( auto it : enumDictionary ) {
        oss << it.first << " = " << it.second << std::endl;
    }

    return oss.str();
}

