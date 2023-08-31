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
        UsingDecl,
        TypedefDecl,
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

    /**
     * @brief Get the name of the given node
     * 
     * @param node json node that represents something that has a name
     * @return std::string string representation of name, empty string if node has no name
     */
    std::string getNodeName(json& node);

    /**
     * @brief Get a reference to the json node representing a list of what's inside this node 
     * 
     * @param node 
     * @return json& 
     */
    json& getNodeInner(json& node);
}