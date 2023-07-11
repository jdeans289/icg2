#pragma once

#include <map>
#include <string>

std::string io_src = std::string(R"(
#include <stddef.h>
#include <stdlib.h>
#include "DataTypeInator.hpp"
#include "Type/EnumDictionary.hpp"
#include "Type/CompositeDataType.hpp"
#include "Value/CompositeValue.hpp"
#include "Type/PrimitiveDataType.hpp"
#include "Type/EnumDataType.hpp"

#include "{{filename}}"

// An Allocator
template <typename T>
void* construct (int num) {
    T* temp = (T*)calloc( num, sizeof(T));
    for (int ii=0 ; ii<num ; ii++) { new( &temp[ii]) T(); }
    return ((void *)temp);
}

// A deAllocator
// Object constructed with placement new must call the destructor explicitly (https://isocpp.org/wiki/faq/dtors#placement-new)
template <typename T>
void destruct (void* address) {
    T* temp = static_cast<T*> (address);
    temp->~T();
}

{{list_class_decl}}

void populate_type_dictionary(DataTypeInator * dataTypeInator) {
    {{list_call_class_decl}}

    dataTypeInator->validateDictionary();
}
)");

std::string call_class_decl = std::string(R"(
    add{{ClassName}}toDictionary(dataTypeInator);
)");

std::string class_decl = std::string(R"(
bool add{{ClassName}}toDictionary(DataTypeInator* dataTypeInator) {

    bool result = false;
    try {
        CompositeDataType* {{ClassName}}TypeSpec =
            new CompositeDataType(dataTypeInator, "{{ClassName}}", sizeof({{ClassName}}), &construct<{{ClassName}}>, &destruct<{{ClassName}}> );
            {{list_field_decl}}

        dataTypeInator->addToDictionary("{{ClassName}}", {{ClassName}}TypeSpec);
    } catch( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
}
)");

std::string field_decl = std::string(R"(
    {{ClassName}}TypeSpec->addRegularMember( "{{FieldName}}", offsetof({{ClassName}}, {{FieldName}}), "{{FieldType}}");
)");

std::map<std::string, std::string> template_dictionary {
    {"top", io_src},
    {"call_class_decl", call_class_decl},
    {"class_decl", class_decl},
    {"field_decl", field_decl},
};
