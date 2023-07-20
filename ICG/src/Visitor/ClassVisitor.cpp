#include "Visitor/ClassVisitor.hpp"
#include "Visitor/FieldVisitor.hpp"

#include "utils.hpp"

ClassVisitor::ClassVisitor() {}


std::vector<ClassInfo> ClassVisitor::getResult() {
    std::vector<ClassInfo> result = { classInfo };
    result.insert(result.end(), nestedClasses.begin(), nestedClasses.end());
    return result;
}

void ClassVisitor::go (CXCursor c) {
    // Pull in the class name
    // Make sure we have the move constructor for ClassInfo
    classInfo = ClassInfo(ICGUtils::getTypeSpelling(c));

    // Traverse the children of this node
    clang_visitChildren(c, forwarding_traverse, this);
}

CXChildVisitResult ClassVisitor::traverse(CXCursor c, CXCursor parent) {
    std::cout << "In Class Traverse--------------------------- " << std::endl;
    std::cout << "\tKind: " << ICGUtils::getKindSpelling(c) << std::endl;
    std::cout << "\tName: " << ICGUtils::getCursorSpelling(c) << std::endl;
    std::cout << "\tType: " << ICGUtils::getTypeSpelling(c) << std::endl;
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
