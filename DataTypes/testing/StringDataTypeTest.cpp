#include <gtest/gtest.h>
#include <stddef.h>
#include <iostream>
#include "DataTypeInator.hh"
#include "Type/StringDataType.hh"
#include "Value/StringValue.hh"


// Framework
class StringDataTypeTest : public ::testing::Test {
    protected:
    // DataTypeInator *dataTypeInator;
    StringDataTypeTest() {  }
    ~StringDataTypeTest() {  }
    void SetUp() {}
    void TearDown() {}
};

TEST_F(StringDataTypeTest, validate) {
    // ARRANGE
    StringDataType type ("std::string");

    // ACT
    bool result = type.validate();

    // ASSERT
    EXPECT_EQ(true, result);
}

TEST_F(StringDataTypeTest, toString) {
    // ARRANGE
    StringDataType type ("std::string");

    // ACT
    std::string result = type.toString();

    // ASSERT
    EXPECT_EQ("std::string", result);
}

TEST_F(StringDataTypeTest, makeDeclaration) {
    // ARRANGE
    StringDataType type ("std::string");

    // ACT
    std::string result = type.makeDeclaration("a");

    // ASSERT
    EXPECT_EQ("std::string a", result);
}

TEST_F(StringDataTypeTest, getValue) {
    // ARRANGE
    std::string my_str = "Hi there!";
    StringDataType type ("std::string");

    // ACT
    Value * result = type.getValue(&my_str);

    // ASSERT
    ASSERT_TRUE(result != NULL);
    StringValue * str_result = dynamic_cast<StringValue *> (result);
    ASSERT_TRUE(str_result != NULL);
    EXPECT_EQ("\"Hi there!\"", str_result->toString());
    EXPECT_EQ("Hi there!", str_result->getRawString());

}

TEST_F(StringDataTypeTest, assignValue) {
    // ARRANGE
    std::string my_str;
    StringDataType type ("std::string");
    StringValue value ("Hi there!");

    // ACT
    type.assignValue(&my_str, &value);

    // ASSERT
    EXPECT_EQ("Hi there!", my_str);
}

TEST_F(StringDataTypeTest, clearValue) {
    // ARRANGE
    std::string my_str = "A string to clear";
    StringDataType type ("std::string");

    // ACT
    type.clearValue(&my_str);

    // ASSERT
    EXPECT_EQ("", my_str);
}

