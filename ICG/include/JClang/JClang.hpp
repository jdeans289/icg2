#pragma once


#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace JClang {

    enum NodeKind {
        ClassDecl,
        FieldDecl,
        NamespaceDecl,
        ClassTemplateDecl,
        ClassTemplateSpecialization,
        TemplateParameterDecl,
        TemplateArgument,
        Unknown
    };

    /**
     * @brief Get the kind of a node
     * 
     * @param node node of json AST
     * @return NodeKind enum of node type
     */
    NodeKind getNodeKind (json& node);

    /**
     * @brief Get the Qualified Type of the field 
     * 
     * @param node json node that represents something that has a type
     * @return std::string fully qualified type
     */
    std::string getQualifiedType (json& node);

    /**
     * @brief Get the base classes of a class declaration
     * 
     * @param class_node json node that represents a class declaration
     * @return std::vector<std::string> string names of parent classes
     */
    std::vector<std::string> getBaseClasses (json& class_node);
}