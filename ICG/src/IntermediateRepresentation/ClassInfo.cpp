#include "IntermediateRepresentation/ClassInfo.hpp"
#include "utils.hpp"

/************************************/
/*       ClassInfo Definitions      */
/************************************/

ClassInfo::ClassInfo() {}
ClassInfo::ClassInfo(std::string n) : name(n) {}

ICGTemplateEngine::Dictionary ClassInfo::toDictionary() const {
    ICGTemplateEngine::Dictionary dictionary;

    std::stringstream fullname;
    fullname << name;
    if (template_args.size() != 0) {
        fullname << "<";
        for (int i = 0; i < template_args.size(); i++) {
            if (i != 0) {
                fullname << ", ";
            }
            fullname << template_args[i];
        }
        fullname << ">";
    }

    dictionary["ClassName"] = fullname.str();
    dictionary["ClassName_mangled"] = ICGUtils::makeVarname(fullname.str());

    return dictionary;
}

// Push the fields
ICGTemplateEngine::ListTokenItems ClassInfo::nextLevel() const {
    std::cout << "Getting next level for class " << name << std::endl;

    ICGTemplateEngine::ListTokenItems my_info_dictionary;
    std::string field_key = "fields";

    for (const auto& field : fields) {
        my_info_dictionary[field_key].push_back(&field);
    }

    std::string bases_key = "bases";

    std::cout << "Num bases: " << base_classes.size() << std::endl;
    for (const auto& base : base_classes) {
        std::cout << "Adding base " << base.toString() << std::endl;
        my_info_dictionary[bases_key].push_back(&base);
    }

    std::cout << "Does this class have a bases list? " << (my_info_dictionary.find("bases") == my_info_dictionary.end() ? "No" : "Yes") << std::endl;

    

    return my_info_dictionary;
}

std::string ClassInfo::toString () const {
    std::stringstream fullname;
    fullname << name;
    if (template_args.size() != 0) {
        fullname << "<";
        for (int i = 0; i < template_args.size(); i++) {
            if (i != 0) {
                fullname << ", ";
            }
            fullname << template_args[i];
        }
        fullname << ">";
    }


    std::stringstream ret;
    ret << "class" << fullname.str();
    
    if (base_classes.size() > 0) {
        ret << " : ";
        for (int i = 0; i < base_classes.size(); i++) {
            if (i != 0) {
                ret << ", ";
            }

            ret << base_classes[i].toString();
        }
    }

    ret << " {\n";
    for (auto field : fields) {
        ret << "\t" << field.toString() << "\n";
    }

    ret << "};\n";
    return ret.str();
}

std::unordered_set<std::string> ClassInfo::getStlMembers() const {

    std::unordered_set<std::string> ret;

    for (auto field : fields) {
        // Did we find an stl type?
        if (ICGUtils::isStlContainer(field.type)) {
            ret.insert(field.type);
        }
    }

    return ret;
}

ClassInfo::BaseInfo::BaseInfo(std::string name) : name(name) {}

ICGTemplateEngine::Dictionary ClassInfo::BaseInfo::toDictionary() const  {
    ICGTemplateEngine::Dictionary dictionary;

    dictionary["BaseClassName"] = name;

    return dictionary;
}


ICGTemplateEngine::ListTokenItems ClassInfo::BaseInfo::nextLevel() const  {
    return ICGTemplateEngine::ListTokenItems();
}

std::string ClassInfo::BaseInfo::toString() const {
    return "base " + name;
}