#include <gtest/gtest.h>
#include <stddef.h>
#include "Utils/MutableDeclaration.hh"
#include <iostream>


TEST(MutableDeclaration, basic) {
    std::string test_string = "int[3][4]";
    MutableDeclaration decl (test_string);

    std::cout << "Test string: " << test_string << std::endl;
    std::cout << "Num dims: " << decl.getNumberOfDimensions() << std::endl;
    for (int i = 0; i < decl.getNumberOfDimensions(); i++) {
        std::cout << "Dim " << i << ": " << decl.getDimensionSize(i) << std::endl;
    }
}

TEST(MutableDeclaration, test1) {

    std::cout << "TEST1: float*baz[3]" << std::endl;
    MutableDeclaration* parsedDeclaration = new MutableDeclaration("float*baz[3]");

    std::string typeSpecString = parsedDeclaration->getTypeSpecifier();
    EXPECT_EQ( "float", typeSpecString );

    std::string varName = parsedDeclaration->getVariableName();
    EXPECT_EQ( "baz", varName );

    unsigned int ndims = parsedDeclaration->getNumberOfDimensions();
    unsigned int dim_0 = parsedDeclaration->getDimensionSize(0);
    unsigned int dim_1 = parsedDeclaration->getDimensionSize(1);

    EXPECT_EQ( 2, ndims );
    EXPECT_EQ( 3, dim_0 );
    EXPECT_EQ(-1, dim_1 );
}


TEST(MutableDeclaration, test2) {

    std::cout << "TEST2: float(*)[3]" << std::endl;
    MutableDeclaration* parsedDeclaration = new MutableDeclaration("float(*)[3]");

    std::string typeSpecString = parsedDeclaration->getTypeSpecifier();
    EXPECT_EQ( "float", typeSpecString );

    std::string varName = parsedDeclaration->getVariableName();
    EXPECT_EQ( "", varName );

    unsigned int ndims = parsedDeclaration->getNumberOfDimensions();
    ASSERT_EQ( 2, ndims );

    unsigned int dim_0 = parsedDeclaration->getDimensionSize(0);
    unsigned int dim_1 = parsedDeclaration->getDimensionSize(1);

    EXPECT_EQ(-1, dim_0 );
    EXPECT_EQ( 3, dim_1 );
}


TEST(MutableDeclaration, test3) {

    MutableDeclaration* parsedDeclaration = new MutableDeclaration("int**[3][4]");

    std::string typeSpecString = parsedDeclaration->getTypeSpecifier();
    EXPECT_EQ( "int", typeSpecString );

    std::string varName = parsedDeclaration->getVariableName();
    EXPECT_EQ( "", varName );

    unsigned int ndims = parsedDeclaration->getNumberOfDimensions();
    ASSERT_EQ( 4, ndims );

    unsigned int dim_0 = parsedDeclaration->getDimensionSize(0);
    unsigned int dim_1 = parsedDeclaration->getDimensionSize(1);
    unsigned int dim_2 = parsedDeclaration->getDimensionSize(2);
    unsigned int dim_3 = parsedDeclaration->getDimensionSize(3);

    EXPECT_EQ( 3, dim_0 );
    EXPECT_EQ( 4, dim_1 );
    EXPECT_EQ(-1, dim_2 );
    EXPECT_EQ(-1, dim_3 );
}

TEST(MutableDeclaration, test4) {

    MutableDeclaration* parsedDeclaration = new MutableDeclaration("int*(*foo)[3][4]");

    std::string typeSpecString = parsedDeclaration->getTypeSpecifier();
    EXPECT_EQ( "int", typeSpecString );

    std::string varName = parsedDeclaration->getVariableName();
    EXPECT_EQ( "foo", varName );

    unsigned int ndims = parsedDeclaration->getNumberOfDimensions();
    ASSERT_EQ( 4, ndims );

    unsigned int dim_0 = parsedDeclaration->getDimensionSize(0);
    unsigned int dim_1 = parsedDeclaration->getDimensionSize(1);
    unsigned int dim_2 = parsedDeclaration->getDimensionSize(2);
    unsigned int dim_3 = parsedDeclaration->getDimensionSize(3);

    EXPECT_EQ(-1, dim_0 );
    EXPECT_EQ( 3, dim_1 );
    EXPECT_EQ( 4, dim_2 );
    EXPECT_EQ(-1, dim_3 );
}

TEST(MutableDeclaration, test5) {

    MutableDeclaration* parsedDeclaration = new MutableDeclaration("int*(*)[3][4]");

    std::string typeSpecString = parsedDeclaration->getTypeSpecifier();
    EXPECT_EQ( "int", typeSpecString );

    std::string varName = parsedDeclaration->getVariableName();
    EXPECT_EQ( "", varName );

    unsigned int ndims = parsedDeclaration->getNumberOfDimensions();
    ASSERT_EQ( 4, ndims );

    unsigned int dim_0 = parsedDeclaration->getDimensionSize(0);
    unsigned int dim_1 = parsedDeclaration->getDimensionSize(1);
    unsigned int dim_2 = parsedDeclaration->getDimensionSize(2);
    unsigned int dim_3 = parsedDeclaration->getDimensionSize(3);

    EXPECT_EQ(-1, dim_0 );
    EXPECT_EQ( 3, dim_1 );
    EXPECT_EQ( 4, dim_2 );
    EXPECT_EQ(-1, dim_3 );
}

TEST(MutableDeclaration, DISABLED_template_basic) {

    MutableDeclaration* parsedDeclaration = new MutableDeclaration("MyClass<int> var");

    std::string typeSpecString = parsedDeclaration->getTypeSpecifier();
    EXPECT_EQ( "int", typeSpecString );

    std::string varName = parsedDeclaration->getVariableName();
    EXPECT_EQ( "var", varName );
}


TEST(MutableDeclaration, basic_get_declarator) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    // No dimensions
    
    MutableDeclaration builder (type);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int a";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, basic_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    // No dimensions
    
    MutableDeclaration builder (type);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, dim1) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int a[5]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, dim1_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int[5]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, multidim) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5, 8, 100, 1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int a[5][8][100][1]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, multidim_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {5, 8, 100, 1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int[5][8][100][1]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, pointer) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int *a";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, pointer_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int*";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, multidim_pointer) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1, -1, -1, -1, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int *****a";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, multidim_pointer_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "a";
    std::vector<int> dims = {-1, -1, -1, -1, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int*****";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, array_of_pointers) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {3, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "float *baz[3]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, array_of_pointers_abstract) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {3, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "float*[3]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, pointer_to_arrays) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {-1, 3};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "float (*baz)[3]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, pointer_to_arrays_abstract) {
    // ARRANGE

    std::string type = "float";
    std::string name = "baz";
    std::vector<int> dims = {-1, 3};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "float(*)[3]";
    EXPECT_EQ(result, expected);
}


// int**[3][4]

TEST(MutableDeclaration, complex1) {
    // ARRANGE

    std::string type = "int";
    std::string name = "baz";
    std::vector<int> dims = {3, 4, -1, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int **baz[3][4]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, complex1_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "baz";
    std::vector<int> dims = {3, 4, -1, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int**[3][4]";
    EXPECT_EQ(result, expected);
}


// "int*(*foo)[3][4]"


TEST(MutableDeclaration, complex2) {
    // ARRANGE

    std::string type = "int";
    std::string name = "foo";
    std::vector<int> dims = {-1, 3, 4, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "int *(*foo)[3][4]";
    EXPECT_EQ(result, expected);
}


TEST(MutableDeclaration, complex2_abstract) {
    // ARRANGE

    std::string type = "int";
    std::string name = "foo";
    std::vector<int> dims = {-1, 3, 4, -1};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getAbstractDeclarator();

    // ASSERT
    std::string expected = "int*(*)[3][4]";
    EXPECT_EQ(result, expected);
}

TEST(MutableDeclaration, invalid_dims) {
    // ARRANGE

    std::string type = "int";
    std::string name = "foo";
    std::vector<int> dims = {-1, 3, 4, -100};
    
    MutableDeclaration builder (type, dims);

    // ACT
    std::string result = builder.getDeclarator(name);

    // ASSERT
    std::string expected = "";
    EXPECT_EQ(result, expected);
}