#include "Visitor/ASTVisitor.hpp"
#include "Visitor/ClassVisitor.hpp"

#include "utils.hpp"

// Entry point
// Start traversal from this node
// This version is the top level so we aren't gonna do anything else here
void AstVisitor::go (CXCursor c) {
    clang_visitChildren(c, forwarding_traverse, this);
}


CXChildVisitResult AstVisitor::traverse(CXCursor c, CXCursor parent) {

    std::cout << "In AST Traverse--------------------------- " << std::endl;
    std::cout << "\tKind: " << ICGUtils::getKindSpelling(c) << std::endl;
    std::cout << "\tName: " << ICGUtils::getCursorSpelling(c) << std::endl;
    std::cout << "\tType: " << ICGUtils::getTypeSpelling(c) << std::endl;
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
