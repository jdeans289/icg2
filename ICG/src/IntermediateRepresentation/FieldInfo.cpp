#include "IntermediateRepresentation/FieldInfo.hpp"
#include "utils.hpp"

/************************************/
/*       FieldInfo Definitions      */
/************************************/

FieldInfo::FieldInfo () {}
FieldInfo::FieldInfo (std::string n, std::string t, AccessLevel a) : type(t), name(n), access_info(a) {}

ICGTemplateEngine::Dictionary FieldInfo::toDictionary() const {
    ICGTemplateEngine::Dictionary dictionary;
    
    dictionary["FieldName"] = name;
    dictionary["FieldType"] = type;
    dictionary["FieldType_mangled"] = ICGUtils::makeVarname(type);

    return dictionary;
}

// Nothing to recurse into
ICGTemplateEngine::ListTokenItems FieldInfo::nextLevel() const {
    ICGTemplateEngine::ListTokenItems empty_list;
    return empty_list;
}

std::string FieldInfo::toString() const {
    return name + ": " + type;
}

// std::ostream& operator<< (std::ostream& stream, const FieldInfo& field) {
//     stream << field.name << "\n\t" << field.type << "\n\t" <<  field.access_info << std::endl;
//     return stream;
// }