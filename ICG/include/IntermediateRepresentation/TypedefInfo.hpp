#pragma once

#include <map>
#include <vector>

#include "utils.hpp"
#include "ICGTemplateEngine/ICGTemplateEngine.hpp"

/**
 * @brief Hold information about a typedef statement
 * 
 */
class TypedefInfo final : public ICGTemplateEngine::recursable {
    public:
    TypedefInfo (std::string alias, std::string existing);

    std::string existing_name;
    std::string alias_name;
    
    ICGTemplateEngine::Dictionary toDictionary() const override;
    ICGTemplateEngine::ListTokenItems nextLevel() const override;
    std::string toString() const override;
};