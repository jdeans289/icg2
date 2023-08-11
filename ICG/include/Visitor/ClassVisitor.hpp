#pragma once

#include <unordered_set>

#include "Visitor/BaseVisitor.hpp"
#include "IntermediateRepresentation/ClassInfo.hpp"
#include "Visitor/FieldVisitor.hpp"

#include "utils.hpp"

/**
 * @brief Visit a ClassDecl node
 * 
 */
class ClassVisitor : public BaseVisitor {
    public: 
    ClassVisitor();

    // Info for this class
    ClassInfo classInfo;

    // Info for possible nested classes
    std::vector<ClassInfo> nestedClasses;
    std::unordered_set<std::string> stlDecls;

    // Result struct
    struct Result {
        std::vector<ClassInfo> classes;
        std::unordered_set<std::string> stlDecls;
    };

    Result getResult();

    void absorbResult(const Result& r);

    void go (CXCursor c) override;
    CXChildVisitResult traverse(CXCursor c, CXCursor parent) override;
};
