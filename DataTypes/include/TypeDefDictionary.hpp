#include <string>
#include <unordered_map>

/**
 * @brief Manage typedef statements
 * 
 */
class TypeDefDictionary {

public:
    /**
     * @brief Register a typedef statement
     * 
     * @param typedefed_name alias name
     * @param canonical_name fully resolved canonical name
     */
    bool registerTypedef (std::string typedefed_name, std::string canonical_name);

    /**
     * @brief Look up the canonical name in the dictionary. If the name is not registered, just return the argument
     * 
     * @param name type name to look up
     * @return std::string canonical name of type
     */
    std::string lookupCanonicalName(std::string name);

    /**
     * @brief Populate this dicionary with defaults. Currently, only std::string -> std::basic_string<char>
     * 
     */
    void addDefaults();

private:
    std::unordered_map<std::string,std::string> typedef_map;
};