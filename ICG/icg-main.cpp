#include <iostream>
#include <string>
#include <vector> 
#include <fstream>
#include <clang-c/Index.h>  // libclang


#include "icg-main.hh"
#include "add-type-templates.hh"
#include "io_template_engine/io_template_engine.hh"

/************************************/
/*       FieldInfo Definitions      */
/************************************/

FieldInfo::FieldInfo (std::string n, std::string t, AccessLevel a) : type(t), name(n), access_info(a) {}

std::map<std::string, std::string> FieldInfo::toDictionary() const {
    std::map<std::string, std::string> dictionary;
    
    dictionary["FieldName"] = name;
    dictionary["FieldType"] = type;

    return dictionary;
}

// Nothing to recurse into
std::vector<const recursable *> FieldInfo::nextLevel() const {
    std::vector<const recursable *> empty_list;
    return empty_list;
}

std::ostream& operator<< (std::ostream& stream, const FieldInfo& field) {
    stream << field.name << "\n\t" << field.type << "\n\t" <<  field.access_info << std::endl;
    return stream;
}

/************************************/
/*       ClassInfo Definitions      */
/************************************/

ClassInfo::ClassInfo(std::string n) : name(n) {}

std::map<std::string, std::string> ClassInfo::toDictionary() const {
    std::map<std::string, std::string> dictionary;
    
    dictionary["ClassName"] = name;

    return dictionary;
}

// Push the fields
std::vector<const recursable *> ClassInfo::nextLevel() const {
    std::vector<const recursable *> my_fields;
    for (const auto& field : fields) {
        my_fields.push_back(&field);
    }
    return my_fields;
}


std::ostream& operator<< (std::ostream& stream, const ClassInfo& class_info) {
    stream << class_info.name;
    stream << "\n{\n";
    for (const auto& field : class_info.fields) {
        stream << field;
    }
    stream << "\n}" << std::endl;
    return stream;
}

/****************************************/
/*       Visitor Class Definitions      */
/****************************************/

class BaseVisitor {
    public:
        virtual CXChildVisitResult traverse(CXCursor c, CXCursor parent) = 0;
};

// Forward to appropriate class method
CXChildVisitResult forwarding_traverse(CXCursor c, CXCursor parent, CXClientData client_data) {        

    // Never go into system libraries
    const CXSourceLocation location = clang_getCursorLocation(c);
    if (clang_Location_isInSystemHeader(location)) {
        return CXChildVisit_Continue;
    }

    BaseVisitor * visitor = reinterpret_cast<BaseVisitor*> (client_data);
    return visitor->traverse(c, parent);
}



class FieldVisitor : public BaseVisitor {
    public: 
    FieldVisitor() {}

    std::vector<FieldInfo> fields;

    virtual CXChildVisitResult traverse(CXCursor c, CXCursor parent) {
        std::cout << "In Field Traverse--------------------------- " << std::endl;
        std::cout << "\tKind: " << getKindSpelling(c) << std::endl;
        std::cout << "\tName: " << getCursorSpelling(c) << std::endl;
        std::cout << "\tType: " << getTypeSpelling(c) << std::endl;
        std::cout << "------------------------------------------\n" << std::endl;
        switch (clang_getCursorKind(c)) {
            case CXCursor_FieldDecl:
                fields.emplace_back(getCursorSpelling(c), getTypeSpelling(c), getAccessLevel(c));
                return CXChildVisit_Continue;
            
            default: ;
        }

        return CXChildVisit_Recurse;
    }
};


class AstVisitor : public BaseVisitor {
    public:
    AstVisitor (CXTranslationUnit * unit_ptr) : unit(unit_ptr) {}

    std::vector<ClassInfo> classes;
    CXTranslationUnit * unit;

    virtual CXChildVisitResult traverse(CXCursor c, CXCursor parent) {
        // Get comments
        std::string comment_str = toStdString(clang_Cursor_getRawCommentText(c));        
        if (comment_str.size() != 0) {
            std::cout << "Comment found: " << comment_str << std::endl;
        }

        // We're looking for top level data types
        // ClassDecl, ClassTemplate, ...
        switch (clang_getCursorKind(c)) {
            case CXCursor_ClassDecl:
                {
                    ClassInfo new_class (getCursorSpelling(c));
                    FieldVisitor fieldVisitor;
                    std::cout << "Visiting fields for class " << new_class.name << std::endl;
                    clang_visitChildren(c, forwarding_traverse, &fieldVisitor);
                    new_class.fields = fieldVisitor.fields;
                    classes.emplace_back(new_class);
                    return CXChildVisit_Continue; 
                }
                break;
            default: ;
                // std::cout << "Unhandled type---------------------------- " << std::endl;
                // std::cout << "\tKind: " << getKindSpelling(c) << std::endl;
                // std::cout << "\tName: " << getCursorSpelling(c) << std::endl;
                // std::cout << "\tType: " << getTypeSpelling(c) << std::endl;
                // std::cout << "------------------------------------------\n" << std::endl;
        }

        return CXChildVisit_Recurse;
    }
};


/****************************************/
/*                Main                  */
/****************************************/

int main(int argc, char ** argv)
{
    CXIndex index = clang_createIndex(0, 0);

    // const int ARG_NUM = 2;
    // char const * command_line_args[ARG_NUM] = {"-fparse-all-comments", "-I/users/jndeans/trick/include/trick"};
    // const char * filename = "/users/jndeans/trick/test/SIM_test_varserv/S_source.hh";


    const int ARG_NUM = 1;
    char const * command_line_args[ARG_NUM] = {"-fparse-all-comments"};
    const char * filename = "test_files/header.hpp";



    CXTranslationUnit unit = clang_parseTranslationUnit(index,
                                                /* source_filename= */      filename, 
                                                /* cmd_line_args= */        command_line_args, 
                                                /* num_cmd_line_args= */    ARG_NUM, 
                                                /* unsaved_files= */        nullptr, 
                                                /* num_unsaved_files= */    0, 
                                                /* options= */              CXTranslationUnit_SkipFunctionBodies);
    if (unit == nullptr)
    {
        std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
        exit(-1);
    }

    AstVisitor visitor(&unit);

    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    clang_visitChildren(cursor, forwarding_traverse, &visitor);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);


    std::vector<const recursable *> classes_list;

    for (const auto& class_info : visitor.classes) {
        classes_list.push_back(&class_info);
        // std::cout << class_info << std::endl;
    }

    std::ofstream outfile;
    outfile.open("io_src.hh");

    template_dictionary["filename"] = filename;

    outfile << big_format(template_dictionary["top"], template_dictionary, classes_list);
    
    outfile.close();
}