#include <iostream>
#include <string>
#include <vector> 
#include <fstream>
#include <clang-c/Index.h>  // libclang

#include "icg_main.hpp"
#include "add_type_templates.hpp"

/************************************/
/*       FieldInfo Definitions      */
/************************************/

FieldInfo::FieldInfo () {}
FieldInfo::FieldInfo (std::string n, std::string t, AccessLevel a) : type(t), name(n), access_info(a) {}

std::map<std::string, std::string> FieldInfo::toDictionary() const {
    std::map<std::string, std::string> dictionary;
    
    dictionary["FieldName"] = name;
    dictionary["FieldType"] = type;
    dictionary["FieldType_mangled"] = makeVarname(type);

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

ClassInfo::ClassInfo() {}
ClassInfo::ClassInfo(std::string n) : name(n) {}

std::map<std::string, std::string> ClassInfo::toDictionary() const {
    std::map<std::string, std::string> dictionary;
    
    dictionary["ClassName"] = name;
    dictionary["ClassName_mangled"] = makeVarname(name);

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

/**
 * @brief A base class for the visitors to traverse the Clang AST
 * @note A correct design will usually go something like:
 *      - go is the entry point of a visitor
 *      - go should pull any useful information out of the current pointer and then call some version of clang_visitChildren
 *      - clang_visitChildren will call traverse with all of the children it finds
 *      - traverse should have a switch statement or something similar to handle any relevant types of nodes found
 *      - Within this switch statement, a Visitor for the associated type should be created with relevant context, and then call visitor.go, and then pull out the result
 *    Calls to clang_visitChildren use the method forwarding_traverse to pass to the correct visitor. Calls should look something like this:
 *          clang_visitChildren(c, forwarding_traverse, &classVisitor);
 */
class BaseVisitor {
    public:
 
        /**
         * @brief This is the hook that will be called from clang_visitChildren via forwarding_traverse
         * @param cursor Current cursor to investigate
         * @param parent Parent cursor
         * @return CXChildVisitResult
         */
        virtual CXChildVisitResult traverse(CXCursor cursor, CXCursor parent) = 0;

        /**
         * @brief The entry point for Clang AST visitors
         * 
         * @param cursor Clang cursor to start visiting from
         */
        virtual void go (CXCursor cursor) = 0;
};

/**
 * @brief Decide if we should recurse into this branch of the tree or exclude it.
 * 
 * @param c Cursor to investigate
 * @return true if cursor is in a system library, false otherwise
 * @todo Implement other excludes here
 */
bool should_exclude (CXCursor c) {
    const CXSourceLocation location = clang_getCursorLocation(c);
    
    // Never go into system libraries
    if (clang_Location_isInSystemHeader(location)) {
        return true;
    }

    return false;
}

/**
 * @brief A free function that should be passed into LibClang's clang_visitChildren method. 
 * 
 * @param c Cursor to visit
 * @param parent parent of c
 * @param client_data A pointer to an instance of the correct visitor class. This is casted to a BaseVisitor and used to call traverse in the correct context.
 * @return CXChildVisitResult 
 */
CXChildVisitResult forwarding_traverse(CXCursor c, CXCursor parent, CXClientData client_data) {        

    if (should_exclude(c)) {
        return CXChildVisit_Continue;
    }

    // Use polymorphism to go to the right visitor
    BaseVisitor * visitor = reinterpret_cast<BaseVisitor*> (client_data);
    return visitor->traverse(c, parent);
}


/**
 * @brief Visit a FieldDecl node
 * 
 */
class FieldVisitor : public BaseVisitor {
    public: 
    FieldVisitor() {}

    FieldInfo field;

    virtual void go (CXCursor c) {
        // Pull in field name, type, access level
        field = FieldInfo(getCursorSpelling(c), getTypeSpelling(c), getAccessLevel(c));

        // I don't think we need to traverse into this anymore?
        // But let's do it anyway and see if we even get nodes printing
        clang_visitChildren(c, forwarding_traverse, this);
    }

    virtual CXChildVisitResult traverse(CXCursor c, CXCursor parent) {

        std::cout << "In Field Traverse--------------------------- " << std::endl;
        std::cout << "\tKind: " << getKindSpelling(c) << std::endl;
        std::cout << "\tName: " << getCursorSpelling(c) << std::endl;
        std::cout << "\tType: " << getTypeSpelling(c) << std::endl;
        std::cout << "------------------------------------------\n" << std::endl;

        switch (clang_getCursorKind(c)) {
            case CXCursor_TypeRef: {
                // If we find a TypeRef, then the Type of this cursor is going to be fully qualified name of the type.
                field.type = getTypeSpelling(c);
                // I don't think this kind of node can have children
                return CXChildVisit_Continue;
            }
        }

        return CXChildVisit_Recurse;
    }
};

/**
 * @brief Visit a ClassDecl node
 * 
 */
class ClassVisitor : public BaseVisitor {
    public: 
    ClassVisitor() {}

    // Info for this class
    ClassInfo classInfo;

    // Info for possible nested classes
    std::vector<ClassInfo> nestedClasses;
    
    std::vector<ClassInfo> getResult() {
        std::vector<ClassInfo> result = { classInfo };
        result.insert(result.end(), nestedClasses.begin(), nestedClasses.end());
        return result;
    }

    virtual void go (CXCursor c) {
        // Pull in the class name
        // Make sure we have the move constructor for ClassInfo
        classInfo = ClassInfo(getTypeSpelling(c));

        // Traverse the children of this node
        clang_visitChildren(c, forwarding_traverse, this);
    }

    virtual CXChildVisitResult traverse(CXCursor c, CXCursor parent) {
        std::cout << "In Class Traverse--------------------------- " << std::endl;
        std::cout << "\tKind: " << getKindSpelling(c) << std::endl;
        std::cout << "\tName: " << getCursorSpelling(c) << std::endl;
        std::cout << "\tType: " << getTypeSpelling(c) << std::endl;
        std::cout << "------------------------------------------\n" << std::endl;

        switch (clang_getCursorKind(c)) {
            case CXCursor_FieldDecl: {
                // Make a field visitor
                FieldVisitor fieldVisitor;
                // Do the thing
                fieldVisitor.go(c);
                // Get the info out
                classInfo.fields.emplace_back(fieldVisitor.field);

                // Continue at sibling nodes
                return CXChildVisit_Continue;
            }
            case CXCursor_ClassDecl: {
                // Found a nested class declaration

                // Make a class visitor
                ClassVisitor nestedClassVisitor;

                // Traverse everything under this
                nestedClassVisitor.go(c);
                
                // Pull out the info that we need from it
                auto result = nestedClassVisitor.getResult();
                nestedClasses.insert(nestedClasses.end(), result.begin(), result.end());

                // Continue at sibling nodes
                return CXChildVisit_Continue;
            }
            default: 
                return CXChildVisit_Recurse;
        }

    }
};

/**
 * @brief Top level AST Visitor
 * 
 */
class AstVisitor : public BaseVisitor {
    public:
    AstVisitor (CXTranslationUnit * unit_ptr) : unit(unit_ptr) {}

    std::vector<ClassInfo> classes;

    CXTranslationUnit * unit;

    // Entry point
    // Start traversal from this node
    // This version is the top level so we aren't gonna do anything else here
    virtual void go (CXCursor c) {
        clang_visitChildren(c, forwarding_traverse, this);
    }


    virtual CXChildVisitResult traverse(CXCursor c, CXCursor parent) {

        std::cout << "In AST Traverse--------------------------- " << std::endl;
        std::cout << "\tKind: " << getKindSpelling(c) << std::endl;
        std::cout << "\tName: " << getCursorSpelling(c) << std::endl;
        std::cout << "\tType: " << getTypeSpelling(c) << std::endl;
        std::cout << "------------------------------------------\n" << std::endl;

        // Get comments
        // std::string comment_str = toStdString(clang_Cursor_getRawCommentText(c));        
        // if (comment_str.size() != 0) {
        //     std::cout << "Comment found: " << comment_str << std::endl;
        // }

        // We're looking for top level data types
        // ClassDecl, ClassTemplate, ...
        // Pass to the correct visitor to handle
        switch (clang_getCursorKind(c)) {
            case CXCursor_ClassDecl:
                {
                    // Make a class visitor
                    ClassVisitor classVisitor;
                    // Traverse everything under this
                    classVisitor.go(c);
                    // Pull out the info that we need from it
                    auto result = classVisitor.getResult();
                    classes.insert(classes.end(), result.begin(), result.end());

                    // Go to the next sibling node of this tree
                    return CXChildVisit_Continue; 
                }
                break;
            default: 
                // std::cout << "Unhandled type---------------------------- " << std::endl;
                // std::cout << "\tKind: " << getKindSpelling(c) << std::endl;
                // std::cout << "\tName: " << getCursorSpelling(c) << std::endl;
                // std::cout << "\tType: " << getTypeSpelling(c) << std::endl;
                // std::cout << "------------------------------------------\n" << std::endl;
                return CXChildVisit_Recurse;

        }
    }
};


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


    // Pull the classes that we found out of the visitor
    std::vector<const recursable *> classes_list;
    for (const auto& class_info : visitor.classes) {
        classes_list.push_back(&class_info);
    }

    // Write the io_<headerfile> file
    std::ofstream outfile;
    outfile.open(makeIOHeaderName(filename));

    // The templates for what the io file should look like live in add-type-templates.hpp
    template_dictionary["filename"] = filename;

    // 'big_format' runs the template engine to construct the io file using the templates and data gathered from the AST
    // TODO: big_format needs a better name @me
    try {
        outfile << big_format(template_dictionary["top"], template_dictionary, classes_list);
    } catch (std::exception& ex) {
        std::cerr << "IO file format failed." << std::endl;
        outfile.close();
        exit(-1);
    }

    outfile.close();
    exit(0);
}