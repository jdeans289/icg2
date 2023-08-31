
#include <map>
#include <vector>
#include <unordered_set>

#include "IntermediateRepresentation/ASTInfo.hpp"
#include "IntermediateRepresentation/STLDeclInfo.hpp"
#include "IntermediateRepresentation/TypedefInfo.hpp"


// Just bundle together all the other things
const std::string ASTInfo::classes_key = "classes";
const std::string ASTInfo::stl_key = "stls";
const std::string ASTInfo::typedef_key = "typedefs";

std::string ASTInfo::toString() const {
    std::stringstream ss;
    ss << "AST Info";

    if (items.find(classes_key) != items.end()) {
        ss << "Classes:" << std::endl;
        for (auto class_info : items.at(classes_key)) {
            ss << class_info->toString() << std::endl;
        } 
    }

    if (items.find(stl_key) != items.end()) {
        ss << "STL Decls: " << std::endl;
        for (auto stl : items.at(stl_key)) {
            ss << stl->toString() << std::endl;
        }
    }

    if (items.find(typedef_key) != items.end()) {
        ss << "TypeDef Decls: " << std::endl;
        for (auto typedef_n : items.at(typedef_key)) {
            ss << typedef_n->toString() << std::endl;
        }
    }

    ss << std::endl;

    return ss.str();
}


void ASTInfo::combine (const ASTInfo& other) {
    // Just combine the stl sets
    stl_decls.insert(other.stl_decls.begin(), other.stl_decls.end());

    // Combine pre-existing entries
    for (auto it : items) {
        auto key = it.first;
        auto other_it = other.items.find(key);

        if (other_it != other.items.end()) {
            items[key].insert(items[key].end(), other.items.at(key).begin(), other.items.at(key).end());
        }
    }

    // This copies all entries that are not in us already, and leaves entries that already exist alone
    items.insert(other.items.begin(), other.items.end());
}

void ASTInfo::add_class_info(ClassInfo * class_info) {
    add_stl_decl(class_info->getStlMembers());
    items[classes_key].push_back(class_info);
}

void ASTInfo::add_stl_decl(std::string stl_name) {
    stl_decls.insert(stl_name);
}

void ASTInfo::add_stl_decl(std::unordered_set<std::string> stl_names) {
    stl_decls.insert(stl_names.begin(), stl_names.end());
}

void ASTInfo::add_typedef (std::string existing_name, std::string alias_name) {
    items[typedef_key].push_back(new TypedefInfo(existing_name, alias_name));
}

ICGTemplateEngine::ListTokenItems ASTInfo::getItems() {
    std::vector<const ICGTemplateEngine::recursable *> stl_recursables;
    for (const auto& stl_name : stl_decls) {
        stl_recursables.push_back (new STLDeclInfo(stl_name));
    }
    items[stl_key] = stl_recursables;

    return items;
}