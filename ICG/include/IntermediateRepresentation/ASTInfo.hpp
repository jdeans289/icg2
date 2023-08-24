#pragma once

#include <map>
#include <vector>
#include <unordered_set>

#include "ICGTemplateEngine/ICGTemplateEngine.hpp"
#include "IntermediateRepresentation/ClassInfo.hpp"

// Just bundle together all the other things

class ASTInfo {
    public:
    // default constructors

    static const std::string classes_key;
    static const std::string stl_key;

    void combine (const ASTInfo& other);

    // void insert (std::string key, ICGTemplateEngine::recursable * item);
    // void insert (std::string key, std::vector<ICGTemplateEngine::recursable *> item);

    void add_class_info(ClassInfo * class_info);

    void add_stl_decl(std::string stl_name);
    void add_stl_decl(std::unordered_set<std::string> stl_names);

    ICGTemplateEngine::ListTokenItems getItems();

    std::string toString() const;

    private:
    ICGTemplateEngine::ListTokenItems items;
    std::unordered_set<std::string> stl_decls;
};