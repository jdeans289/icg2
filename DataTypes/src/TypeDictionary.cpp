#include <sstream>
#include <iostream>
#include <stack>
#include <algorithm>

#include "Type/SpecifiedPrimitiveDataType.hpp"
#include "Type/StringDataType.hpp"
#include "Type/ArrayDataType.hpp"
#include "Type/PointerDataType.hpp"

#include "Utils/ParsedDeclaration.hpp"

#include "TypeDictionary.hpp"




// MEMBER FUNCTION
TypeDictionary::TypeDictionary() {

}

void TypeDictionary::addBuiltinTypes() {
    // Add builtin types.
    addTypeDefinition("void", new SpecifiedPrimitiveDataType<void>() );
    addTypeDefinition("char", new SpecifiedPrimitiveDataType<char>() );
    addTypeDefinition("short", new SpecifiedPrimitiveDataType<short>() );
    addTypeDefinition("int", new SpecifiedPrimitiveDataType<int>() );
    addTypeDefinition("long", new SpecifiedPrimitiveDataType<long>() );
    addTypeDefinition("wchar_t", new SpecifiedPrimitiveDataType<wchar_t>() );
    addTypeDefinition("long long", new SpecifiedPrimitiveDataType<long long>() );
    addTypeDefinition("unsigned char", new SpecifiedPrimitiveDataType<unsigned char>() );
    addTypeDefinition("unsigned short", new SpecifiedPrimitiveDataType<unsigned short>() );
    addTypeDefinition("unsigned int", new SpecifiedPrimitiveDataType<unsigned int>() );
    addTypeDefinition("unsigned long", new SpecifiedPrimitiveDataType<unsigned long>() );
    addTypeDefinition("unsigned long long", new SpecifiedPrimitiveDataType<unsigned long long>() );
    addTypeDefinition("float", new SpecifiedPrimitiveDataType<float>() );
    addTypeDefinition("double", new SpecifiedPrimitiveDataType<double>() );
    // FIXME: add the other goofy types (e.g., uint8 ) that were added via the old lexical analyzer.

    addTypeDefinition("std::string", new StringDataType());
}


// MEMBER FUNCTION
const BaseType* TypeDictionary::lookup(std::string name ) {

    ParsedDeclaration decl(name);
    auto qualifiedNameParts = decl.getQualifiedNameParts();

    // If it's a built in type, we won't have to deal with qualified name stuff
    if (qualifiedNameParts.empty()) {
        auto it = typeDictionary.find(name);
        if (it == typeDictionary.end()) {
            return NULL;
        }
        return it->second;
    } else {
        return lookup(qualifiedNameParts);
    }
}

const BaseType* TypeDictionary::lookup(std::deque<std::string> nameParts) {
    // If the size is 1, then we're in the correct dictionary. Just lookup and return.
    if (nameParts.size() == 1) {
        auto it = typeDictionary.find(nameParts.front());
        if (it == typeDictionary.end()) {
            return NULL;
        }
        return it->second;
    }

    // Pop off the front and recurse into the namespace dictionary.
    std::string thisNamespace = nameParts.front();
    nameParts.pop_front();

    auto it = namespaceDictionary.find(thisNamespace);
    if (it == namespaceDictionary.end()) {
        return NULL;
    }

    return it->second->lookup(nameParts);
}

// MEMBER FUNCTION
void TypeDictionary::addTypeDefinition(std::string name, BaseType * typeSpec)  {

    const BaseType* preExistingDataType = lookup(name);

    if ( preExistingDataType == NULL ) {
        // Add to dictionary
        ParsedDeclaration decl(name);
        auto qualifiedNameParts = decl.getQualifiedNameParts();

        // If it's a built in type, we won't have to deal with qualified name stuff
        if (qualifiedNameParts.empty()) {
            typeDictionary[name] = typeSpec;
        } else {
            addTypeDefinition(qualifiedNameParts, typeSpec);
        }

    } else {
        std::stringstream error_stream ;
        error_stream << "ERROR: Attempt to re-define type \"" << name << "\"" << std::endl;
        throw std::logic_error( error_stream.str());
    }
}

// PRIVATE MEMBER FUNCTION
// Assumes lookup check has already been done
void TypeDictionary::addTypeDefinition(std::deque<std::string> nameParts, BaseType * typeSpec)  {

    if (nameParts.size() == 1) {
        // If the qualified name parts only has 1 item, we're at the base.
        typeDictionary[nameParts.front()] = typeSpec;
        return;
    }

    std::string thisNamespace = nameParts.front();
    nameParts.pop_front();

    // Otherwise, we need to look up the namespace
    TypeDictionary * nextLevelDict = NULL;

    // Create a new namespace dictionary or look up an existing one
    auto it = namespaceDictionary.find(thisNamespace);
    if (it == namespaceDictionary.end()) {
        // Create a new dictionary for this namespace
        nextLevelDict = new TypeDictionary;
        namespaceDictionary[thisNamespace] = nextLevelDict;
    } else {
        nextLevelDict = it->second;
    }

    nextLevelDict->addTypeDefinition(nameParts, typeSpec);
}

// MEMBER FUNCTION
bool TypeDictionary::validate() {

    is_valid = true;
    for ( auto it  = typeDictionary.begin();
          it != typeDictionary.end();
          it++ ) {

         BaseType* dataType = it->second;
         is_valid = is_valid && dataType->validate();
    }
    if (!is_valid) {
        std::cerr << "TypeDictionary contains invalidate data-types.";
    }
    return is_valid;
}

std::string addTabs(std::string str, int numTabs) {
    std::string tabStr;
    for (int i = 0; i < numTabs; i++) {
        tabStr += "\t";
    }

    // Always put it at the beginning
    str = tabStr + str;

    // Count "\n" chars
    int numNewlines = std::count(str.begin(), str.end(), '\n');

    // Replace them all except the last one

    // Modified version of  
    // https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
    size_t startPos = 0;
    std::string from = "\n";
    std::string to = "\n" + tabStr;
    int replaceCounter = 0;

    while((startPos = str.find("\n", startPos)) != std::string::npos && replaceCounter++ < numNewlines - 1) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Handles case where 'to' is a substring of 'from'
    }

    return str;
}

// MEMBER FUNCTION
std::string TypeDictionary::toString() {
    std::ostringstream oss;

    // Print members of this dictionary
    for ( const auto& it : typeDictionary ) {
        oss << it.first << " = " << it.second->toString() << std::endl;
    }

    // Print members of nested namespace dictionary
    for ( const auto& it : namespaceDictionary ) {
        oss << "namespace " << it.first << " {" << std::endl;
        oss << addTabs(it.second->toString(), 1);
        oss << "}" << std::endl;
    }

    return oss.str();
}

// MEMBER FUNCTION
TypeDictionary::~TypeDictionary() {

    // Delete all DataTypes in the map.
    for ( auto it = typeDictionary.begin(); it != typeDictionary.end(); it++ ) {
        delete it->second;
    }
}








