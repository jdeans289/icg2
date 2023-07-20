#include "IntermediateRepresentation/ClassInfo.hpp"
#include "utils.hpp"

/************************************/
/*       ClassInfo Definitions      */
/************************************/

ClassInfo::ClassInfo() {}
ClassInfo::ClassInfo(std::string n) : name(n) {}

ICGTemplateEngine::Dictionary ClassInfo::toDictionary() const {
    ICGTemplateEngine::Dictionary dictionary;
    
    dictionary["ClassName"] = name;
    dictionary["ClassName_mangled"] = ICGUtils::makeVarname(name);

    return dictionary;
}

// Push the fields
std::vector<const ICGTemplateEngine::recursable *> ClassInfo::nextLevel() const {
    std::vector<const ICGTemplateEngine::recursable *> my_fields;
    for (const auto& field : fields) {
        my_fields.push_back(&field);
    }
    return my_fields;
}

// std::ostream& operator<< (std::ostream& stream, const ClassInfo& class_info) {
//     stream << class_info.name;
//     stream << "\n{\n";
//     for (const auto& field : class_info.fields) {
//         stream << field;
//     }
//     stream << "\n}" << std::endl;
//     return stream;
// }