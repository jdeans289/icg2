#pragma once

#include "Visitor/BaseVisitor.hpp"



class ClassTemplateVisitor : public BaseVisitor {
    public:
    ClassTemplateVisitor();

    void go (CXCursor c) override;
    CXChildVisitResult traverse(CXCursor c, CXCursor parent) override;
};


