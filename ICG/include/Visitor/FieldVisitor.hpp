#pragma once

#include "Visitor/BaseVisitor.hpp"

#include "IntermediateRepresentation/ClassInfo.hpp"

/**
 * @brief Visit a FieldDecl node
 * 
 */
class FieldVisitor : public BaseVisitor {
    public: 
    FieldVisitor();

    FieldInfo field;

    void go (CXCursor c) override;
    CXChildVisitResult traverse(CXCursor c, CXCursor parent) override;
};