#pragma once

#include <map>
#include <string>
#include <stdexcept>
#include "Type/BaseType.hpp"


/**
 Stores name / typespecifier pairs.
 */
class TypeDictionary {

    public:

    TypeDictionary();

    /**
     Get the BaseType of the for the typedef'ed name.
     */
    const BaseType* lookup(std::string typeName);

    /**
     Add a type definiton to the dictionary.
     */
    void addTypeDefinition(std::string name, BaseType * typeSpec);


    /**
     */
    bool validate();

    /**
     Dump the entire TypeDictionary to a std::string.
     */
    std::string toString();

    /**
      Destructor.
     */
    ~TypeDictionary();

    // helper
    // This probably doesn't belong here
    void addBuiltinTypes();
    
    private:
    bool is_valid;

    std::map<std::string, BaseType*> typeDictionary;
    std::map<std::string, TypeDictionary *> namespaceDictionary;

    // Helpers to avoid having to parse the declaration multiple times
    void addTypeDefinition(std::deque<std::string> nameParts, BaseType * typeSpec);
    const BaseType* lookup(std::deque<std::string> nameParts);


};
