#pragma once

#include "Visitor/BaseVisitor.hpp"
#include "IntermediateRepresentation/ClassInfo.hpp"

/**
 * @brief Top level AST Visitor
 * 
 */
class AstVisitor : public BaseVisitor {
    public:
    AstVisitor (CXTranslationUnit * unit_ptr) : unit(unit_ptr) {}

    std::vector<ClassInfo> classes;

    CXTranslationUnit * unit;

    void go (CXCursor c) override;
    CXChildVisitResult traverse(CXCursor c, CXCursor parent) override;
};