// #include "DataType.hh"
#include "TypeDictionary.hh"
#include "PrimitiveDataType.hh"
#include "ParsedDeclaration.hh"
#include "DeclarationBuilder.hh"
#include "ArrayDataType.hh"
#include "PointerDataType.hh"


#include <sstream>
#include <iostream>
#include <stack>

// MEMBER FUNCTION
TypeDictionary::TypeDictionary() {

    // Add builtin types.
    addTypeDefinition("void", new PrimitiveDataType<void>() );
    addTypeDefinition("char", new PrimitiveDataType<char>() );
    addTypeDefinition("short", new PrimitiveDataType<short>() );
    addTypeDefinition("int", new PrimitiveDataType<int>() );
    addTypeDefinition("long", new PrimitiveDataType<long>() );
    addTypeDefinition("wchar_t", new PrimitiveDataType<wchar_t>() );
    addTypeDefinition("long long", new PrimitiveDataType<long long>() );
    addTypeDefinition("unsigned char", new PrimitiveDataType<unsigned char>() );
    addTypeDefinition("unsigned short", new PrimitiveDataType<unsigned short>() );
    addTypeDefinition("unsigned int", new PrimitiveDataType<unsigned int>() );
    addTypeDefinition("unsigned long", new PrimitiveDataType<unsigned long>() );
    addTypeDefinition("unsigned long long", new PrimitiveDataType<unsigned long long>() );
    addTypeDefinition("float", new PrimitiveDataType<float>() );
    addTypeDefinition("double", new PrimitiveDataType<double>() );
    // FIXME: add the other goofy types (e.g., uint8 ) that were added via the old lexical analyzer.

}


// MEMBER FUNCTION

// I think this violates the SRP
// should move most of this into a separate class, and just keep this class purely managing the TypeDictionary
const BaseType* TypeDictionary::lookup(std::string name ) {

    TypeDictionaryIterator it = typeDictionary.find(name);

    if (it == typeDictionary.end()) {
        return NULL;
    }

    return it->second;
}

// MEMBER FUNCTION
void TypeDictionary::addTypeDefinition(std::string name, BaseType * typeSpec)  {

    const BaseType* preExistingDataType = lookup(name);

    if ( preExistingDataType == NULL ) {
        typeDictionary[name] = typeSpec;
    } else {
        std::stringstream error_stream ;
        error_stream << "ERROR: Attempt to re-define type \"" << name << "\"" << std::endl;
        throw std::logic_error( error_stream.str());
    }
}

// MEMBER FUNCTION
bool TypeDictionary::validate() {

    is_valid = true;
    for ( TypeDictionaryIterator it  = typeDictionary.begin();
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

// MEMBER FUNCTION
std::string TypeDictionary::toString() {
    std::ostringstream oss;

    for ( TypeDictionaryIterator it = typeDictionary.begin();
          it != typeDictionary.end();
          it++ ) {

        oss << it->first << " = " << it->second->toString() << std::endl;
    }

    return oss.str();
}

// MEMBER FUNCTION
TypeDictionary::~TypeDictionary() {

    // Delete all DataTypes in the map.
    for ( TypeDictionaryIterator it = typeDictionary.begin(); it != typeDictionary.end(); it++ ) {
        delete it->second;
    }
}








