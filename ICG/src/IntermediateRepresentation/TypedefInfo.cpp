#include "IntermediateRepresentation/TypedefInfo.hpp"
#include "utils.hpp"

/************************************/
/*       TypedefInfo Definitions      */
/************************************/

TypedefInfo::TypedefInfo (std::string alias, std::string existing) : alias_name(alias), existing_name(existing) {}

ICGTemplateEngine::Dictionary TypedefInfo::toDictionary() const {
    ICGTemplateEngine::Dictionary dictionary;
    
    dictionary["AliasName"] = alias_name;
    dictionary["ExistingName"] = existing_name;

    return dictionary;
}

// Nothing to recurse into
ICGTemplateEngine::ListTokenItems TypedefInfo::nextLevel() const {
    return ICGTemplateEngine::ListTokenItems();
}

std::string TypedefInfo::toString() const {
    return "typedef " + existing_name + " " + alias_name;
}