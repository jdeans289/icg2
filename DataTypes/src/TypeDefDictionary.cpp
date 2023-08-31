#include "TypeDefDictionary.hpp"

bool TypeDefDictionary::registerTypedef (std::string typedefed_name, std::string canonical_name) {
    // Should we do a lookup until canonical name is no longer in the dictionary? that might be smart actually
    // I think we can get the canonical name fully from icg though, so shouldn't need to

    typedef_map.insert(std::pair<std::string, std::string>(typedefed_name, canonical_name));
    return true;
}

std::string TypeDefDictionary::lookupCanonicalName(std::string name) {
    auto lookup = typedef_map.find(name);

    // If it's not in the map, then given name is canonical.
    if (lookup == typedef_map.end()) {
        return name;
    }

    // Just return result of lookup. No need to worry about checking anything here
    return lookup->second;
}

void TypeDefDictionary::addDefaults() {
    registerTypedef("std::string", "std::basic_string<char>");
}