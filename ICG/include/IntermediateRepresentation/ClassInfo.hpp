#pragma once

#include <map>
#include <vector>

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

    // Rule of 5 is default

    std::string name;
    std::vector<FieldInfo> fields;

    ICGTemplateEngine::Dictionary toDictionary() const override;
    ICGTemplateEngine::ListTokenItems nextLevel() const override;

    // friend std::ostream& operator<< (std::ostream& stream, const ClassInfo& str);
};
