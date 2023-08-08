#include <gtest/gtest.h>
#include <stddef.h>
#include "Utils/ParsedDeclaration.hpp"
#include <iostream>

TEST(ParsedDeclaration, basic) {
    std::string test_string = "int[3][4]";
    ParsedDeclaration parsedDeclaration (test_string);

    // ASSERT
    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "int", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "", varName );

    std::vector<int> expected_dims = {3, 4};
    EXPECT_EQ(expected_dims, parsedDeclaration.getDims());

}

TEST(ParsedDeclaration, test1) {

    // ARRANGE
    // ACT
    ParsedDeclaration parsedDeclaration("float*baz[3]");

    // ASSERT
    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "float", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "baz", varName );

    std::vector<int> expected_dims = {3, -1};
    EXPECT_EQ(expected_dims, parsedDeclaration.getDims());

}


TEST(ParsedDeclaration, test2) {

    ParsedDeclaration parsedDeclaration("float(*)[3]");

    // ASSERT
    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "float", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "", varName );

    std::vector<int> expected_dims = {-1, 3};
    EXPECT_EQ(expected_dims, parsedDeclaration.getDims());
}


TEST(ParsedDeclaration, test3) {

    ParsedDeclaration parsedDeclaration("int**[3][4]");


    // ASSERT
    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "int", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "", varName );

    std::vector<int> expected_dims = {3, 4, -1, -1};
    EXPECT_EQ(expected_dims, parsedDeclaration.getDims());
}

TEST(ParsedDeclaration, test4) {

    ParsedDeclaration parsedDeclaration("int*(*foo)[3][4]");

    // ASSERT
    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "int", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "foo", varName );

    std::vector<int> expected_dims = {-1, 3, 4, -1};
    EXPECT_EQ(expected_dims, parsedDeclaration.getDims());
}

TEST(ParsedDeclaration, test5) {

    ParsedDeclaration parsedDeclaration("int*(*)[3][4]");

    // ASSERT
    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "int", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "", varName );

    std::vector<int> expected_dims = {-1, 3, 4, -1};
    EXPECT_EQ(expected_dims, parsedDeclaration.getDims());
}

TEST(ParsedDeclaration, namespace_string) {

    ParsedDeclaration parsedDeclaration("std::string var");

    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "std::string", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "var", varName );

    auto qualifiedList = parsedDeclaration.getQualifiedNameParts();
    std::deque<std::string> expected = {"std", "string"};
    EXPECT_EQ(expected, qualifiedList);
}

TEST(ParsedDeclaration, arbitrary_namespace) {

    ParsedDeclaration parsedDeclaration("A::B::C::D var");

    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "A::B::C::D", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "var", varName );

    auto qualifiedList = parsedDeclaration.getQualifiedNameParts();
    std::deque<std::string> expected = {"A", "B", "C", "D"};
    EXPECT_EQ(expected, qualifiedList);
}

TEST(ParsedDeclaration, template_basic) {

    ParsedDeclaration parsedDeclaration("MyClass<int> var");

    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "MyClass<int>", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "var", varName );
}

TEST(ParsedDeclaration, stl_vector) {

    ParsedDeclaration parsedDeclaration("std::vector<int> var");

    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "std::vector<int>", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "var", varName );
}

TEST(ParsedDeclaration, stl_nested) {

    ParsedDeclaration parsedDeclaration("std::vector<std::stack<int>> var");

    std::string typeSpecString = parsedDeclaration.getTypeSpecifier();
    EXPECT_EQ( "std::vector<std::stack<int>>", typeSpecString );

    std::string varName = parsedDeclaration.getVariableName();
    EXPECT_EQ( "var", varName );
}
