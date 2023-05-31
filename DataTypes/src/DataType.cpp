#include "DataType.hh"
#include <sstream>

/*
    type-name: type-specifier
             | type-specifier declarator
             ;

    declarator : pointer
               | pointer direct-declarator
               | direct-declarator
               ;

    direct-declarator: identifier
                     | ( declarator )
                     | [ constant-exp ]
                     | [ ]
                     | direct-declarator [ constant-exp ]
                     | direct-declarator [ ]
                     ;
*/

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

// void DataType::checkpointValue(std::ostream &s, std::string var_name, void *address) const {
//     throw std::logic_error("checkpointValue is not yet implemented for this type.");
// }

void DataType::accept (DataTypeVisitor* visitor) const {
    throw std::logic_error("accept is not yet implemented for this type.");
}
