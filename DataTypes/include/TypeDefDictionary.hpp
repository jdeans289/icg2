#include <string>
#include <unordered_map>

/**
 * @brief Manage typedef statements
 * 
 */
class TypeDefDictionary {

public:
    bool registerTypedef (std::string typedefed_name, std::string canonical_name);
    std::string lookupCanonicalName(std::string name);

    void addDefaults();

private:
    std::unordered_map<std::string,std::string> typedef_map;
};