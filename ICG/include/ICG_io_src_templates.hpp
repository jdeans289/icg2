#pragma once

#include <map>
#include <string>

std::string io_src = std::string(R"(
#include <stddef.h>
#include <stdlib.h>
#include <iostream>
#include "DataTypeInator.hpp"
#include "Type/SpecifiedCompositeType.hpp"
#include "Type/SpecifiedSequenceDataType.hpp"

#include "{{filename}}"

{{list_classes_class_type_decl}}

void populate_type_dictionary(DataTypeInator * dataTypeInator) {
    {{list_classes_call_class_decl}}

    {{list_stls_stl_decl}}

    dataTypeInator->validateDictionary();
}
)");

std::string call_class_decl = std::string(R"(dataTypeInator->addToDictionary("{{ClassName}}", new SpecifiedCompositeType<{{ClassName}}>);
)");

std::string class_type_decl = std::string(R"(
template <>
class SpecifiedCompositeType<{{ClassName}}> : public CompositeDataType {

    public:
    SpecifiedCompositeType() : CompositeDataType("{{ClassName}}", sizeof({{ClassName}}), &construct_composite<{{ClassName}}>, &destruct_composite<{{ClassName}}>) {}

    template<typename Derived>
    static MemberMap applyMembersToDerived () {
        using type_to_add = {{ClassName}};

        MemberMap derived_member_map = {
            {{list_fields_derived_field_decl}}
        };

        {{list_bases_add_base_members_to_derived}}

        return derived_member_map;
    }    

    MemberMap& getMemberMap () {
        using type_to_add = {{ClassName}};

        static bool initialized = false;
        static MemberMap member_map = {
            {{list_fields_field_decl}}
        };

        if (!initialized) {
            {{list_bases_add_base_members_to_self}}

            initialized = true;
        }

        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<{{ClassName}}>*> (this))->getMemberMap();
    }
};
)");


std::string derived_field_decl = std::string (R"({"{{FieldName}}", StructMember("{{FieldName}}", "{{FieldType}}", offsetof(Derived, type_to_add::{{FieldName}}))},
)");

std::string field_decl = std::string(R"({"{{FieldName}}", StructMember("{{FieldName}}", "{{FieldType}}", offsetof(type_to_add, {{FieldName}}))},
)");

std::string stl_decl = std::string(R"(dataTypeInator->addToDictionary("{{STLName}}", new SpecifiedSequenceDataType<{{STLName}}>("{{STLName}}"));
)");

std::string add_base_members_to_self = std::string(R"(
auto derived_members = SpecifiedCompositeType<{{BaseClassName}}>::applyMembersToDerived<{{ClassName}}>();
member_map.insert(derived_members.begin(), derived_members.end());
)");

std::string add_base_members_to_derived = std::string(R"(
auto derived_members = SpecifiedCompositeType<{{BaseClassName}}>::applyMembersToDerived<Derived>();
derived_member_map.insert(derived_members.begin(), derived_members.end());
)");

std::map<std::string, std::string> template_dictionary {
    {"top", io_src},
    {"call_class_decl", call_class_decl},
    {"class_type_decl", class_type_decl},
    {"field_decl", field_decl},
    {"derived_field_decl", derived_field_decl},
    {"add_base_members_to_self", add_base_members_to_self},
    {"add_base_members_to_derived", add_base_members_to_derived},
    {"stl_decl", stl_decl}
};
