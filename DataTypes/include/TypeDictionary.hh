#pragma once

#include <map>
#include <string>
#include <stdexcept>
#include "Type/BaseType.hh"


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

    private:
    bool is_valid;
    std::map<std::string, BaseType*> typeDictionary;
    typedef std::map<std::string, BaseType*>::iterator TypeDictionaryIterator;
};
