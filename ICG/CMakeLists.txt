cmake_minimum_required(VERSION 3.1)

set(ICG_SRC
    icg_main.cpp
    utils.cpp
    ICGTemplateEngine/ICGTemplateEngine.cpp
    IntermediateRepresentation/ClassInfo.cpp
    IntermediateRepresentation/FieldInfo.cpp
    IntermediateRepresentation/ClassTemplateInfo.cpp
    Visitor/BaseVisitor.cpp
    Visitor/ASTVisitor.cpp
    Visitor/ClassVisitor.cpp
    Visitor/FieldVisitor.cpp
    Visitor/ClassTemplateVisitor.cpp
)

list(TRANSFORM ICG_SRC PREPEND src/)

add_executable (icg ${ICG_SRC})

target_include_directories(icg PUBLIC include)
target_link_libraries(icg PUBLIC clang)