#pragma once

#include <map>
#include <vector>
#include <unordered_set>

#include "utils.hpp"
#include "ICGTemplateEngine/ICGTemplateEngine.hpp"
#include "IntermediateRepresentation/FieldInfo.hpp"

/**
 * @brief Hold information about a class declaration
 * 
 */
class ClassInfo final : public ICGTemplateEngine::recursable {
public:
    ClassInfo();
    ClassInfo(std::string n);


    /**
     * @brief Represent a base class. 
     * 
     */
    class BaseInfo : public ICGTemplateEngine::recursable {
    public:
        BaseInfo(std::string name);
        std::string name;
        ICGTemplateEngine::Dictionary toDictionary() const override;
        ICGTemplateEngine::ListTokenItems nextLevel() const override;
        std::string toString() const override;
    };
    

    // Rule of 5 is default

    std::string name;
    std::vector<FieldInfo> fields;
    std::vector<BaseInfo> base_classes;
    std::vector<std::string> template_args;

    ICGTemplateEngine::Dictionary toDictionary() const override;
    ICGTemplateEngine::ListTokenItems nextLevel() const override;
    std::string toString() const override;

    std::unordered_set<std::string> getStlMembers() const;

};
