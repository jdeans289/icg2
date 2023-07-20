#pragma once

#include <map>
#include <vector>

#include "utils.hpp"
#include "ICGTemplateEngine/ICGTemplateEngine.hpp"

/**
 * @brief Hold information about a field declaration
 * 
 */
class FieldInfo final : public ICGTemplateEngine::recursable {
    public:
    FieldInfo ();
    FieldInfo (std::string n, std::string t, AccessLevel a);

    // Rule of 5 is default

    std::string type;
    std::string name;
    AccessLevel access_info;
    StorageClass storage_class;
    
    ICGTemplateEngine::Dictionary toDictionary() const override;
    std::vector<const ICGTemplateEngine::recursable *> nextLevel() const override;

    // friend std::ostream& operator<< (std::ostream& stream, const FieldInfo& str);
};