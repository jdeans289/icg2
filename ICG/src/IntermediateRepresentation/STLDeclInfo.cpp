#include "IntermediateRepresentation/STLDeclInfo.hpp"
#include "utils.hpp"

/************************************/
/*       FieldInfo Definitions      */
/************************************/

STLDeclInfo::STLDeclInfo () {}
STLDeclInfo::STLDeclInfo (std::string n) : name(n) {}

ICGTemplateEngine::Dictionary STLDeclInfo::toDictionary() const {
    ICGTemplateEngine::Dictionary dictionary;
    
    dictionary["STLName"] = name;

    return dictionary;
}

// Nothing to recurse into
ICGTemplateEngine::ListTokenItems STLDeclInfo::nextLevel() const {
    return ICGTemplateEngine::ListTokenItems();
}

std::string STLDeclInfo::toString() const {
    return name;
}