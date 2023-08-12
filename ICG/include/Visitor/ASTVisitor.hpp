#pragma once

#include "Visitor/BaseVisitor.hpp"
#include "IntermediateRepresentation/ClassInfo.hpp"
#include <unordered_set>

/**
 * @brief Top level AST Visitor
 * 
 */
class AstVisitor : public BaseVisitor {
    public:
    AstVisitor (CXTranslationUnit * unit_ptr) : unit(unit_ptr) {}

    std::vector<const ICGTemplateEngine::recursable *> getClassInfo();
    std::vector<const ICGTemplateEngine::recursable *> getSTLDeclInfo();

    void go (CXCursor c) override;
    CXChildVisitResult traverse(CXCursor c, CXCursor parent) override;

    private:
    std::vector<ClassInfo> classes;
    std::unordered_set<std::string> stlDecls;

    CXTranslationUnit * unit;
};