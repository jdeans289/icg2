#include <gtest/gtest.h>
#include <stddef.h>
#include <iostream>
#include "DataTypeInator.hpp"
#include "Type/StringDataType.hpp"
#include "Value/StringValue.hpp"


// Framework
class StringDataTypeTest : public ::testing::Test {
    protected:
    StringDataTypeTest() {  }
    ~StringDataTypeTest() {  }
    void SetUp() {}
    void TearDown() {}
};

TEST_F(StringDataTypeTest, validate) {
    // ARRANGE
    StringDataType type ;

    // ACT
    bool result = type.validate();

    // ASSERT
    EXPECT_EQ(true, result);
}

TEST_F(StringDataTypeTest, toString) {
    // ARRANGE
    StringDataType type ;

    // ACT
    std::string result = type.toString();

    // ASSERT
    EXPECT_EQ("std::string", result);
}

TEST_F(StringDataTypeTest, makeDeclaration) {
    // ARRANGE
    StringDataType type ;

    // ACT
    std::string result = type.makeDeclaration("a");

    // ASSERT
    EXPECT_EQ("std::string a", result);
}

// TEST_F(StringDataTypeTest, getValue) {
//     // ARRANGE
//     std::string my_str = "Hi there!";
//     StringDataType type ;

//     // ACT
//     Value * result = type.getValue(&my_str);

//     // ASSERT
//     ASSERT_TRUE(result != NULL);
//     StringValue * str_result = dynamic_cast<StringValue *> (result);
//     ASSERT_TRUE(str_result != NULL);
//     EXPECT_EQ("\"Hi there!\"", str_result->toString());
//     EXPECT_EQ("Hi there!", str_result->getRawString());

// }

// TEST_F(StringDataTypeTest, assignValue) {
//     // ARRANGE
//     std::string my_str;
//     StringDataType type ;
//     StringValue value ("Hi there!");

//     // ACT
//     type.assignValue(&my_str, &value);

//     // ASSERT
//     EXPECT_EQ("Hi there!", my_str);
// }

// TEST_F(StringDataTypeTest, clearValue) {
//     // ARRANGE
//     std::string my_str = "A string to clear";
//     StringDataType type ;

//     // ACT
//     type.clearValue(&my_str);

//     // ASSERT
//     EXPECT_EQ("", my_str);
// }

