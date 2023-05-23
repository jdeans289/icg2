#include <stddef.h>
#include <iostream>

#include <gtest/gtest.h>

#include "DeclarationBuilder.hh"

TEST(DeclarationBuilder, basic) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    // No dimensions
    
    DeclarationBuilder builder (type);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int a";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, basic_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    // No dimensions
    
    DeclarationBuilder builder (type);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, dim1) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int a[5]";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, dim1_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int[5]";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, multidim) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5, 8, 100, 1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int a[5][8][100][1]";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, multidim_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5, 8, 100, 1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int[5][8][100][1]";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, pointer) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int *a";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, pointer_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int*";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, multidim_pointer) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1, -1, -1, -1, -1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int *****a";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, multidim_pointer_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1, -1, -1, -1, -1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int*****";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, array_of_pointers) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {3, -1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "float *baz[3]";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, array_of_pointers_abstract) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {3, -1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "float*[3]";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, pointer_to_arrays) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {-1, 3};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "float (*baz)[3]";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, pointer_to_arrays_abstract) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {-1, 3};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "float(*)[3]";
    EXPECT_EQ(result, expected);
}


// int**[3][4]

TEST(DeclarationBuilder, complex1) {
    // ARRANGE

    std::string type = "int";
    std::string name = "baz";
    std::vector<int> dims = {3, 4, -1, -1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int **baz[3][4]";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, complex1_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "baz";
    std::vector<int> dims = {3, 4, -1, -1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int**[3][4]";
    EXPECT_EQ(result, expected);
}


// "int*(*foo)[3][4]"


TEST(DeclarationBuilder, complex2) {
    // ARRANGE

    std::string type = "int";
    std::string name = "foo";
    std::vector<int> dims = {-1, 3, 4, -1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int *(*foo)[3][4]";
    EXPECT_EQ(result, expected);
}


TEST(DeclarationBuilder, complex2_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "foo";
    std::vector<int> dims = {-1, 3, 4, -1};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int*(*)[3][4]";
    EXPECT_EQ(result, expected);
}

TEST(DeclarationBuilder, invalid_dims) {
    // ARRANGE

    std::string type = "int";
    std::string name = "foo";
    std::vector<int> dims = {-1, 3, 4, -100};
    
    DeclarationBuilder builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "";
    EXPECT_EQ(result, expected);
}