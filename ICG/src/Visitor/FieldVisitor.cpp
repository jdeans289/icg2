
#include "Visitor/FieldVisitor.hpp"

#include "utils.hpp"

FieldVisitor::FieldVisitor() {}

void FieldVisitor::go (CXCursor c) {
    // Pull in field name, type, access level
    field = FieldInfo(ICGUtils::getCursorSpelling(c), ICGUtils::getTypeSpelling(c), ICGUtils::getAccessLevel(c));

    // I don't think we need to traverse into this anymore?
    // But let's do it anyway and see if we even get nodes printing
    clang_visitChildren(c, forwarding_traverse, this);
}

CXChildVisitResult FieldVisitor::traverse(CXCursor c, CXCursor parent) {

    std::cout << "In Field Traverse--------------------------- " << std::endl;
    std::cout << "\tKind: " << ICGUtils::getKindSpelling(c) << std::endl;
    std::cout << "\tName: " << ICGUtils::getCursorSpelling(c) << std::endl;
    std::cout << "\tType: " << ICGUtils::getTypeSpelling(c) << std::endl;
    std::cout << "------------------------------------------\n" << std::endl;

    switch (clang_getCursorKind(c)) {
        case CXCursor_TypeRef: {
            // If we find a TypeRef, then the Type of this cursor is going to be fully qualified name of the type.
            field.type = ICGUtils::getTypeSpelling(c);
            // I don't think this kind of node can have children
            return CXChildVisit_Continue;
        }
    }

    return CXChildVisit_Recurse;
}
