#include <iostream>
#include <string>
#include <vector> 
#include <fstream>
#include <clang-c/Index.h>  // libclang

#include "ICGTemplateEngine/ICGTemplateEngine.hpp"
#include "IntermediateRepresentation/ClassInfo.hpp"
#include "Visitor/ASTVisitor.hpp"

#include "ICG_io_src_templates.hpp"


/****************************************/
/*                Main                  */
/****************************************/

int main(int argc, char ** argv) {

    if (argc != 2) {
        std::cerr << "Usage: icg <headerfile>" << std::endl;
        exit(-1);
    }

    const int ARG_NUM = 1;
    char const * command_line_args[ARG_NUM] = {"-fparse-all-comments"};
    const char * filename = argv[1];

    CXIndex index = clang_createIndex(0, 0);


    CXTranslationUnit unit = clang_parseTranslationUnit(index,
                                                /* source_filename= */      filename, 
                                                /* cmd_line_args= */        command_line_args, 
                                                /* num_cmd_line_args= */    ARG_NUM, 
                                                /* unsaved_files= */        nullptr, 
                                                /* num_unsaved_files= */    0, 
                                                /* options= */              CXTranslationUnit_SkipFunctionBodies);

    if (unit == nullptr) {
        std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
        exit(-1);
    }

    // This is the data structure we'll use to gather information about the types defined in the header
    AstVisitor visitor(&unit);

    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    // Do the thing!
    visitor.go(cursor);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    // Pull the results that we found out of the visitor
    ICGTemplateEngine::ListTokenItems top_level_objects_list;

    std::string classes_key = "classes";
    top_level_objects_list[classes_key] = visitor.getClassInfo();

    std::string stl_key = "stls";
    top_level_objects_list[stl_key] = visitor.getSTLDeclInfo();

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