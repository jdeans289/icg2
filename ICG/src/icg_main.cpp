#include <iostream>
#include <string>
#include <vector> 
#include <fstream>
#include <nlohmann/json.hpp>

#include "ICGTemplateEngine/ICGTemplateEngine.hpp"
#include "IntermediateRepresentation/ClassInfo.hpp"
#include "IntermediateRepresentation/ASTInfo.hpp"
#include "IntermediateRepresentation/ScopeInfo.hpp"

#include "ASTFilter/ASTFilter.hpp"

#include "ICG_io_src_templates.hpp"

using json = nlohmann::json;

/********************************************/
/*               Scrape ASTs                */
/********************************************/

std::string pull_qualified_type(json& field_node) {
    if (field_node["type"].contains("desugaredQualType")) {
        return field_node["type"]["desugaredQualType"];
    }
    return field_node["type"]["qualType"];
}

bool is_class_definition (json& node) {
    bool result;
    if (node["kind"] == "CXXRecordDecl" ) {
        if ( node["tagUsed"] == "class") {
            if (node.contains("isImplicit")) {
                if (node["isImplicit"] == true) {
                    return false;
                }
            } else {
                return true;
            }
        }
    }

    return false;


}


ASTInfo scrape_class_info (json& class_node, ScopeInfo& scope) {
    ASTInfo result;

    ClassInfo * info = new ClassInfo;
    info->name = scope.make_scoped_name(class_node["name"]);

    for (auto item : class_node["inner"]) {
        if (item["kind"] == "FieldDecl") {
            // Just a field declarations
            FieldInfo field;
            field.name = item["name"];
            field.type = pull_qualified_type(item);
            info->fields.push_back(field);
        }

        if (is_class_definition(item)) {
            // Nested class
            scope.push_qualifier(class_node["name"]);
            result.combine(scrape_class_info(item, scope));
            scope.pop_qualifier();
        }
    }

    result.add_class_info(info);

    std::cout << "\n\nAST scraped from class: \n" << result.toString() << std::endl; 

    return result;
    
}


ASTInfo scrape_class_template_info (json& class_template_node, ScopeInfo& scope) {
    ASTInfo result;

    std::string template_name = scope.make_scoped_name(class_template_node["name"]);
    int num_template_args = 0;

    for (auto item : class_template_node["inner"]) {
        if (item["kind"] == "TemplateTypeParmDecl") {
            num_template_args++;
        }
        
        if (item["kind"] == "ClassTemplateSpecializationDecl") {
            ClassInfo * info = new ClassInfo;
            info->name = template_name;
            for (auto template_item : item["inner"]) {

                if (template_item["kind"] == "TemplateArgument") {
                    info->template_args.push_back(pull_qualified_type(template_item));
                }

                if (template_item["kind"] == "FieldDecl") {
                    FieldInfo field;
                    field.name = template_item["name"];
                    field.type = pull_qualified_type(template_item);

                    info->fields.push_back(field);
                }
            }
            result.add_class_info(info);
        }
    }

    std::cout << "\n\nAST scraped from class template: \n" << result.toString() << std::endl;; 

    return result;                                                                                                                                                                                                                                                                                                                                       
}


ASTInfo scrape_ast (json& ast_node, ScopeInfo scope) {

    ASTInfo total_result;

    for (auto item : ast_node["inner"]) {
        if (item["kind"] == "ClassTemplateDecl") {
            
            // Process a class template declaration
            total_result.combine(scrape_class_template_info(item, scope));
            std::cout << "\n\nTotal AST after scraping from class template: \n" << total_result.toString() << std::endl;; 

        } else if (is_class_definition(item)) {

            // Process a class declaration
            total_result.combine(scrape_class_info(item, scope));
            std::cout << "\n\nTotal AST after scraping from class: \n" << total_result.toString() << std::endl;; 

        } else if (item["kind"] == "NamespaceDecl") {
            if (item.contains("inner")) {

                scope.push_qualifier(item["name"]);
                total_result.combine(scrape_ast(item, scope));
                scope.pop_qualifier();

                std::cout << "\n\nTotal AST after namespace: \n" << total_result.toString() << std::endl;; 
            }
        } else {
            std::cout << "Encountered unknown kind: " << item["kind"] << std::endl;
        }
    }

    return total_result;
}

/****************************************/
/*                Main                  */
/****************************************/

int main(int argc, char ** argv) {

    if (argc != 2) {
        std::cerr << "Usage: icg <headerfile>" << std::endl;
        exit(-1);
    }

    std::string filename = argv[1];

    json ast = ASTFilter::generateFilteredAST(filename);

    ScopeInfo scope;
    ASTInfo ast_info = scrape_ast(ast, scope);
    std::cout << "**********************************************************" << std::endl;
    std::cout << "Total AST scraped: \n" << ast_info.toString() << std::endl;; 

    ICGTemplateEngine::ListTokenItems top_level_objects_list = ast_info.getItems();


    // std::unordered_set<std::string> stlDecls;

    // for (auto item : ast["inner"]) {
    //     if (item["kind"] == "ClassTemplateDecl") {
    //         auto results = scrape_class_template_info(item);
    //         top_level_objects_list[classes_key].insert(top_level_objects_list[classes_key].end(), results.begin(), results.end());
    //         for (auto class_info : results) {
    //             auto this_stl_decls = class_info->getStlMembers();
    //             stlDecls.insert(this_stl_decls.begin(), this_stl_decls.end());
    //         }
    //     }

    //     if (item["kind"] == "CXXRecordDecl" && item["tagUsed"] == "class") {
    //         auto results = scrape_class_info(item);
    //         top_level_objects_list[classes_key].insert(top_level_objects_list[classes_key].end(), results.begin(), results.end());
    //         for (auto class_info : results) {
    //             auto this_stl_decls = class_info->getStlMembers();
    //             stlDecls.insert(this_stl_decls.begin(), this_stl_decls.end());
    //         }
    //     }
    // }

    // std::vector<const ICGTemplateEngine::recursable *> stl_recursables;
    // for (const auto& stl_name : stlDecls) {
    //     stl_recursables.push_back (new STLDeclInfo(stl_name));
    // }
    // top_level_objects_list[stl_key] = stl_recursables;

    // Write the io_<headerfile> file
    std::ofstream outfile;
    outfile.open(ICGUtils::makeIOHeaderName(filename));

    // The templates for what the io file should look like live in add-type-templates.hpp
    template_dictionary["filename"] = filename;

    // Runs the template engine to construct the io file using the templates and data gathered from the AST
    try {
        outfile << ICGTemplateEngine::process(template_dictionary, top_level_objects_list);
    } catch (std::exception& ex) {
        std::cerr << "IO file format failed: " << ex.what() << std::endl;
        outfile.close();
        exit(-1);
    }

    outfile.close();
    exit(0);
}