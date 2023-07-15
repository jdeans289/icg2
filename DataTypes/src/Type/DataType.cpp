#include "Type/DataType.hpp"
#include <sstream>

// VIRTUAL MEMBER FUNCTION
// NOTE: PointerDataType and ArrayDataType overrides this member-function.
std::string DataType::makeDeclaration(std::string declarator) const {
    std::stringstream ss;
    if (declarator.empty()) {
        ss << getTypeSpecName();
    } else {
        ss << getTypeSpecName() << " " << declarator ;
    }
    return ss.str();
}

// VIRTUAL MEMBER FUNCTION
std::string DataType::toString() const {
    return makeDeclaration("");
}