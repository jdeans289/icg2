cmake_minimum_required(VERSION 3.6)

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

if( NOT LLVM_ROOT STREQUAL "" ) 
    message("LLVM Root is specified: ${LLVM_ROOT}")
    target_link_libraries(icg PUBLIC clang)
    target_include_directories(icg PUBLIC ${LLVM_ROOT}/include)
    target_link_directories(icg PUBLIC ${LLVM_ROOT}/lib)

endif()

target_link_libraries(icg PUBLIC clang)

# Get the include directories for the target.
get_target_property(ICG_INCLUDES icg INCLUDE_DIRECTORIES)
message(STATUS "ICG include directories: '${ICG_INCLUDES}'")




