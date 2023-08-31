#include "JClang/Traversal.hpp"
#include "JClang/JClang.hpp"

namespace JClang {


    ASTInfo traverseAST (json& ast) {
        Scope scope;
        return scrape_ast(ast, scope);
    }

    ASTInfo scrape_ast (json& ast_node, Scope& scope) {

        ASTInfo total_result;
        for (auto item : getNodeInner(ast_node)) {

            switch (getNodeKind(item)) {
                case ClassTemplateDecl: {
                    total_result.combine(scrape_class_template_decl_info(item, scope));
                }
                break;
                case ClassDecl: {
                    total_result.combine(scrape_class_info(item, scope));
                }
                break;
                case NamespaceDecl: {
                    scope.push_qualifier(getNodeName(item));
                    total_result.combine(scrape_ast(item, scope));
                    scope.pop_qualifier();
                }
                break;
                case UsingDecl: {
                    // ignore
                }
                break;
                case TypedefDecl: {
                    total_result.combine(scrape_typedef_info(item, scope));
                }
                break;
                default:
                    std::cout << "Encountered unknown kind: " << item["kind"] << std::endl;
            }
        }

        return total_result;
    }

    ASTInfo scrape_class_info (json& class_node, Scope& scope) {
        ASTInfo result;

        ClassInfo * info = new ClassInfo;
        info->name = scope.make_scoped_name(getNodeName(class_node));

        // Get base classes
        for (auto base_class_name : getBaseClasses(class_node)) {
            info->base_classes.emplace_back(base_class_name);
        }

        scope.push_qualifier(getNodeName(class_node));

        // Iterate through contents
        for (auto item : getNodeInner(class_node)) {
            
            switch (getNodeKind(item)) {
                case FieldDecl: {
                    info->fields.push_back(scrape_field_decl_info(item, scope));
                }
                break;
                case ClassDecl: {
                    result.combine(scrape_class_info(item, scope));
                }
                break;
                case TypedefDecl: {
                    result.combine(scrape_typedef_info(item, scope));
                }
                break;
                default: ;
            }
        }

        scope.pop_qualifier();

        result.add_class_info(info);

        return result;
    }


    ASTInfo scrape_class_template_decl_info (json& class_template_node, Scope& scope) {
        ASTInfo result;

        std::string template_name = scope.make_scoped_name(getNodeName(class_template_node));
        int num_template_args = 0;

        for (auto item : getNodeInner(class_template_node)) {

            switch (getNodeKind(item)) {
                case TemplateParameterDecl: {
                    num_template_args++;
                }
                break;
                case ClassTemplateSpecialization: {
                    result.combine(scrape_class_template_spec_info(item, scope));
                }
                break;
                default: ;
                
            }
        }

        return result;                                                                                                                                                                                                                                                                                                                                       
    }

    ASTInfo scrape_class_template_spec_info (json& class_template_node, Scope& scope) {
        ASTInfo result;

        ClassInfo * info = new ClassInfo;
        info->name = scope.make_scoped_name(getNodeName(class_template_node));

        for (auto template_item : getNodeInner(class_template_node)) {
            switch (getNodeKind(template_item)) {
                case TemplateArgument: {
                    info->template_args.push_back(getQualifiedType(template_item));
                }
                break;
                case FieldDecl: {
                    info->fields.push_back(scrape_field_decl_info(template_item, scope));
                }
                break;
                default: ;
            }
        }

        result.add_class_info(info);

        return result;
    }


    ASTInfo scrape_typedef_info (json& typedef_node, Scope& scope) {
        ASTInfo result;
        std::string alias_name = scope.make_scoped_name(getNodeName(typedef_node));
        std::string existing_name = getQualifiedType(typedef_node);
        result.add_typedef(existing_name, alias_name);
        return result;
    }


    FieldInfo scrape_field_decl_info (json& field_node, Scope& scope) {
        FieldInfo field;
        field.name = getNodeName(field_node);
        field.type = getQualifiedType(field_node);
        return field;
    }
}