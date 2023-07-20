#pragma once

#include <map>
#include <vector>

#include "utils.hpp"
#include "ICGTemplateEngine/ICGTemplateEngine.hpp"

/**
 * @brief Hold info about a ClassTemplate
 * 
 */
class ClassTemplateInfo final : public ICGTemplateEngine::recursable {
public:
    ClassTemplateInfo();
    
    std::string name;

    std::vector<std::string> template_parameters;

    std::vector<FieldInfo> concrete_fields;
    std::vector<FieldInfo> templated_fields;

    ICGTemplateEngine::Dictionary toDictionary() const override;
    std::vector<const ICGTemplateEngine::recursable *> nextLevel() const override;
}
