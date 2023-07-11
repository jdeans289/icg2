#include "Utils/MutableVariableName.hpp"
#include <gtest/gtest.h>

TEST(MutableVariableNameTest, toString_basic) {
    // ARRANGE
    MutableVariableName stack;
    stack.pushName("var");

    // ACT
    std::string actual = stack.toString();


    // ASSERT
    std::string expected = "var";
    EXPECT_EQ(expected, actual);
}

TEST(MutableVariableNameTest, toString_composite) {
    // ARRANGE
    MutableVariableName stack;
    stack.pushName("var");
    stack.pushName("a");
    stack.pushName("b");
    stack.pushName("c");


    // ACT
    std::string actual = stack.toString();


    // ASSERT
    std::string expected = "var.a.b.c";
    EXPECT_EQ(expected, actual);
}

TEST(MutableVariableNameTest, toString_indexes) {
    // ARRANGE
    MutableVariableName stack;
    stack.pushName("var");
    stack.pushIndex(0);
    stack.pushIndex(100);
    stack.pushIndex(6);


    // ACT
    std::string actual = stack.toString();


    // ASSERT
    std::string expected = "var[0][100][6]";
    EXPECT_EQ(expected, actual);
}

TEST(MutableVariableNameTest, toString_mixed) {
    // ARRANGE
    MutableVariableName stack;
    stack.pushName("var");
    stack.pushName("a");
    stack.pushIndex(0);
    stack.pushIndex(100);
    stack.pushName("b");
    stack.pushIndex(6);


    // ACT
    std::string actual = stack.toString();


    // ASSERT
    std::string expected = "var.a[0][100].b[6]";
    EXPECT_EQ(expected, actual);
}

void compare_vecs(std::vector<std::string> expected, std::vector<std::string> actual) {
    ASSERT_EQ(expected.size(), actual.size());
    for (int i = 0; i < actual.size(); i++) {
        EXPECT_EQ(expected[i], actual[i]);
    } 
}

TEST(MutableVariableNameTest, parse_basic) {
    // ARRANGE
    MutableVariableName stack ("var");

    // ACT
    std::vector<std::string> actual = stack.getElements();
    std::vector<std::string> expected = {"var"};

    // ASSERT
    compare_vecs(expected, actual);
}

TEST(MutableVariableNameTest, parse_composite) {
    // ARRANGE
    MutableVariableName stack ("var.a.b.c");

    // ACT
    std::vector<std::string> actual = stack.getElements();
    std::vector<std::string> expected = {"var", "a", "b", "c"};

    // ASSERT
    compare_vecs(expected, actual);
}

TEST(MutableVariableNameTest, parse_index) {
    // ARRANGE
    MutableVariableName stack ("var[0][100][6]");

    // ACT
    std::vector<std::string> actual = stack.getElements();
    std::vector<std::string> expected = {"var", "[0]", "[100]", "[6]"};

    // ASSERT
    compare_vecs(expected, actual);
}

TEST(MutableVariableNameTest, parse_mixed) {
    // ARRANGE
    MutableVariableName stack ("var.a[0][100].b[6]");

    // ACT
    std::vector<std::string> actual = stack.getElements();
    std::vector<std::string> expected = {"var", "a", "[0]", "[100]", "b", "[6]"};

    // ASSERT
    compare_vecs(expected, actual);
}


TEST(MutableVariableNameTest, parse_index_exception) {
    // ARRANGE
    try {
        MutableVariableName stack ("var.a[nan]");
        FAIL() << "Expected an exception to be thrown.";
    } catch (std::exception& e) {
        EXPECT_TRUE (strlen(e.what()) > 0 );
    }
}

TEST(MutableVariableNameTest, parse_negative_index_exception) {
    // ARRANGE
    try {
        MutableVariableName stack ("var.a[-1]");
        FAIL() << "Expected an exception to be thrown.";
    } catch (std::exception& e) {
        EXPECT_TRUE (strlen(e.what()) > 0 );
    }
}

TEST(MutableVariableNameTest, parse_missing_end_bracket) {
    // ARRANGE
    try {
        MutableVariableName stack ("var.a[0");
        FAIL() << "Expected an exception to be thrown.";
    } catch (std::exception& e) {
        EXPECT_TRUE (strlen(e.what()) > 0 );
    }
}
