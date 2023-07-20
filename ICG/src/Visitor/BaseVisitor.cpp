#include "Visitor/BaseVisitor.hpp"


bool should_exclude (CXCursor c) {
    const CXSourceLocation location = clang_getCursorLocation(c);
    
    // Never go into system libraries
    if (clang_Location_isInSystemHeader(location)) {
        return true;
    }

    return false;
}

CXChildVisitResult forwarding_traverse(CXCursor c, CXCursor parent, CXClientData client_data) {        

    if (should_exclude(c)) {
        return CXChildVisit_Continue;
    }

    // Use polymorphism to go to the right visitor
    BaseVisitor * visitor = reinterpret_cast<BaseVisitor*> (client_data);
    return visitor->traverse(c, parent);
}