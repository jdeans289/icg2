
#include <iostream>
#include "JClang/JClang.hpp"

namespace JClang {

    static json empty_node;

    NodeKind getNodeKind (json& node) {

        if (!node.contains("kind")) {
            return Unknown;
        }

        if (node["kind"] == "CXXRecordDecl" ) {
            if ( node["tagUsed"] == "class" || node["tagUsed"] == "struct" ) {
                if (node.contains("isImplicit")) {
                    if (node["isImplicit"] == true) {
                        return Unknown;
                    }
                } else {
                    return ClassDecl;
                }
            }
        }

        if (node["kind"] == "FieldDecl") {
            return FieldDecl;
        }

        if (node["kind"] == "TemplateTypeParmDecl") {
            return TemplateParameterDecl;
        }

        if (node["kind"] == "ClassTemplateSpecializationDecl") {
            return ClassTemplateSpecialization;
        }

        if (node["kind"] == "TemplateArgument") {
            return TemplateArgument;
        }

        if (node["kind"] == "ClassTemplateDecl") {
            return ClassTemplateDecl;
        }

        if (node["kind"] == "NamespaceDecl") {
            return NamespaceDecl;
        }

        if (node["kind"] == "UsingDecl") {
            return UsingDecl;
        }

        if (node["kind"] == "TypedefDecl") {
            return TypedefDecl;
        }

        return Unknown;
    }

    std::string getQualifiedType (json& field_node) {
        if (!field_node.contains("type")) {
            std::cerr << "Attempted to get type of node that does not have a type." << std::endl;
            return "ERROR_TYPE";
        }

        // We want "desugaredQualType" if it exists
        if (field_node["type"].contains("desugaredQualType")) {
            return field_node["type"]["desugaredQualType"];
        }

        // "qualType" otherwise
        if (field_node["type"].contains("qualType")) {
            return field_node["type"]["qualType"];
        }        
        
        std::cerr << "Type could not be extracted from node: " << field_node["type"].dump(4) << std::endl;
        return "ERROR_TYPE";
    }

    std::vector<std::string> getBaseClasses (json& class_node) {
        if (getNodeKind(class_node) != ClassDecl) {
            std::cerr << "Attempted to get base classes of a node that is not a class declaration." << std::endl; 
            return std::vector<std::string>();
        }

        if (!class_node.contains("bases")) {
            return std::vector<std::string>();
        }

        std::vector<std::string> result;
        for ( auto item : class_node["bases"] ) {
            result.push_back(getQualifiedType(item));
        }

        std::cout << std::endl;

        return result;
    }

    std::string getNodeName(json& node) {
        if (!node.contains("name")) {
            std::cerr << "Attempted to get name of node that does not contain a name." << std::endl;
            return "";
        }

        return node["name"];
    }

    json& getNodeInner(json& node) {
        if (!node.contains("inner")) {
            return empty_node;
        }

        return node["inner"];
    }

}