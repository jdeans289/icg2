#include "Type/EnumDictionary.hpp"
#include "Type/PrimitiveDataType.hpp"
#include "Type/NormalStructMember.hpp"
#include "DataTypeTestSupport.hpp"

#include "gtest/gtest.h"

#include "Algorithm/LookupAddressByNameVisitor.hpp"


class LookupAddressByNameVisitorTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    LookupAddressByNameVisitorTest() {

    }

    ~LookupAddressByNameVisitorTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

TEST_F(LookupAddressByNameVisitorTest, basic) {
    // ARRANGE
    int x;
    PrimitiveDataType<int> int_data_type;

    LookupAddressByNameVisitor visitor(&x, "");
    bool status = visitor.go(&int_data_type);

    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&x, result.address);
    EXPECT_EQ(int_data_type.toString(), result.type->toString());
}

TEST_F(LookupAddressByNameVisitorTest, int_error) {
    // ARRANGE
    int x;
    PrimitiveDataType<int> int_data_type;

    LookupAddressByNameVisitor visitor(&x, "no_such_thing");
    bool status = visitor.go(&int_data_type);

    ASSERT_FALSE(status);
}

TEST_F(LookupAddressByNameVisitorTest, composite1) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassOne");

    ClassOne var_to_search;

    // ACT
    LookupAddressByNameVisitor visitor(&var_to_search, "b");
    bool status = visitor.go(data_type);

    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search.b, result.address);

    PrimitiveDataType<double> double_data_type;
    EXPECT_EQ(double_data_type.toString(), result.type->toString());
}

TEST_F(LookupAddressByNameVisitorTest, composite2) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * c2_data_type = dataTypeInator.resolve("ClassTwo");

    ClassTwo var_to_search;

    // ACT
    LookupAddressByNameVisitor visitor(&var_to_search, "c1.b");
    bool status = visitor.go(c2_data_type);

    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search.c1.b, result.address);

    PrimitiveDataType<double> double_data_type;
    EXPECT_EQ(double_data_type.toString(), result.type->toString());
}

TEST_F(LookupAddressByNameVisitorTest, composite4) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * c2_data_type = dataTypeInator.resolve("ClassTwo");

    ClassTwo var_to_search;

    // ACT
    LookupAddressByNameVisitor visitor(&var_to_search, "c1");
    bool status = visitor.go(c2_data_type);

    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search.c1, result.address);
    const DataType * c1_data_type = dataTypeInator.resolve("ClassOne");
    EXPECT_EQ(c1_data_type->toString(), result.type->toString());
}

TEST_F(LookupAddressByNameVisitorTest, composite3) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * c2_data_type = dataTypeInator.resolve("ClassTwo");

    ClassTwo var_to_search;

    // ACT
    LookupAddressByNameVisitor visitor(&var_to_search, "c1");
    bool status = visitor.go(c2_data_type);

    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search.c1, result.address);
    const DataType * c1_data_type = dataTypeInator.resolve("ClassOne");
    EXPECT_EQ(c1_data_type->toString(), result.type->toString());
}

TEST_F(LookupAddressByNameVisitorTest, composite_non_existent_member) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * c2_data_type = dataTypeInator.resolve("ClassTwo");

    ClassTwo var_to_search;

    // ACT
    LookupAddressByNameVisitor visitor(&var_to_search, "c1.no_such_member");
    bool status = visitor.go(c2_data_type);

    // ASSERT
    ASSERT_FALSE(status);
}

TEST_F(LookupAddressByNameVisitorTest, array) {
    // ARRANGE
    const DataType * data_type = dataTypeInator.resolve("int[5]");
    int var_to_search[5];

    // ACT
    LookupAddressByNameVisitor visitor(&var_to_search, "[3]");
    bool status = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search[3], result.address);
    PrimitiveDataType<int> int_type;
    EXPECT_EQ(int_type.toString(), result.type->toString());
}

TEST_F(LookupAddressByNameVisitorTest, multidim_array) {
    // ARRANGE
    const DataType * data_type = dataTypeInator.resolve("int[5][4][3]");
    int var_to_search[5][4][3];

    // ACT
    LookupAddressByNameVisitor visitor(&var_to_search, "[3][2][1]");
    bool status = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search[3][2][1], result.address);
    PrimitiveDataType<int> int_type;
    EXPECT_EQ(int_type.toString(), result.type->toString());
}


TEST_F(LookupAddressByNameVisitorTest, multidim_array2) {
    // ARRANGE
    const DataType * data_type = dataTypeInator.resolve("int[5][4][3]");
    int var_to_search[5][4][3];

    // ACT
    LookupAddressByNameVisitor visitor(&var_to_search, "[3][2]");
    bool status = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search[3][2], result.address);
    const DataType * expected_data_type = dataTypeInator.resolve("int[3]");
    EXPECT_EQ(expected_data_type->toString(), result.type->toString());
}

TEST_F(LookupAddressByNameVisitorTest, composite_array) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * data_type = dataTypeInator.resolve("ClassTwo[5]");
    ClassTwo var_to_search[5];

    // ACT
    LookupAddressByNameVisitor visitor(&var_to_search, "[3].c1.b");
    bool status = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search[3].c1.b, result.address);
    PrimitiveDataType<double> int_type;
    EXPECT_EQ(int_type.toString(), result.type->toString());
}
