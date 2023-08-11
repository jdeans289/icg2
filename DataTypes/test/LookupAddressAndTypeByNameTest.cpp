#include "Type/EnumDictionary.hpp"
#include "Type/SpecifiedPrimitiveDataType.hpp"
#include "Type/SpecifiedSequenceDataType.hpp"
#include "Type/NormalStructMember.hpp"
#include "DataTypeTestSupport.hpp"

#include "gtest/gtest.h"

#include "Algorithm/LookupAddressAndTypeByName.hpp"


class LookupAddressAndTypeByNameTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    LookupAddressAndTypeByNameTest() {}
    ~LookupAddressAndTypeByNameTest() {}

    void SetUp() {}
    void TearDown() {}
};

namespace LookupAddressAndTypeByName {

TEST_F(LookupAddressAndTypeByNameTest, basic) {
    // ARRANGE
    int x;
    SpecifiedPrimitiveDataType<int> int_data_type;

    LookupAddressAndTypeByNameVisitor visitor(&x, "");
    bool status = visitor.go(&int_data_type);

    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&x, result.address);
    EXPECT_EQ(int_data_type.toString(), result.type->toString());
}

TEST_F(LookupAddressAndTypeByNameTest, int_error) {
    // ARRANGE
    int x;
    SpecifiedPrimitiveDataType<int> int_data_type;

    LookupAddressAndTypeByNameVisitor visitor(&x, "no_such_thing");
    bool status = visitor.go(&int_data_type);

    ASSERT_FALSE(status);
}

TEST_F(LookupAddressAndTypeByNameTest, composite1) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassOne");

    ClassOne var_to_search;

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "b");
    bool status = visitor.go(data_type);

    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search.b, result.address);

    SpecifiedPrimitiveDataType<double> double_data_type;
    EXPECT_EQ(double_data_type.toString(), result.type->toString());
}

TEST_F(LookupAddressAndTypeByNameTest, composite2) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * c2_data_type = dataTypeInator.resolve("ClassTwo");

    ClassTwo var_to_search;

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "c1.b");
    bool status = visitor.go(c2_data_type);

    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search.c1.b, result.address);

    SpecifiedPrimitiveDataType<double> double_data_type;
    EXPECT_EQ(double_data_type.toString(), result.type->toString());
}

TEST_F(LookupAddressAndTypeByNameTest, composite4) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * c2_data_type = dataTypeInator.resolve("ClassTwo");

    ClassTwo var_to_search;

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "c1");
    bool status = visitor.go(c2_data_type);

    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search.c1, result.address);
    const DataType * c1_data_type = dataTypeInator.resolve("ClassOne");
    EXPECT_EQ(c1_data_type->toString(), result.type->toString());
}

TEST_F(LookupAddressAndTypeByNameTest, composite3) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * c2_data_type = dataTypeInator.resolve("ClassTwo");

    ClassTwo var_to_search;

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "c1");
    bool status = visitor.go(c2_data_type);

    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search.c1, result.address);
    const DataType * c1_data_type = dataTypeInator.resolve("ClassOne");
    EXPECT_EQ(c1_data_type->toString(), result.type->toString());
}

TEST_F(LookupAddressAndTypeByNameTest, composite_non_existent_member) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * c2_data_type = dataTypeInator.resolve("ClassTwo");

    ClassTwo var_to_search;

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "c1.no_such_member");
    bool status = visitor.go(c2_data_type);

    // ASSERT
    ASSERT_FALSE(status);
}

TEST_F(LookupAddressAndTypeByNameTest, array) {
    // ARRANGE
    const DataType * data_type = dataTypeInator.resolve("int[5]");
    int var_to_search[5];

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "[3]");
    bool status = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search[3], result.address);
    SpecifiedPrimitiveDataType<int> int_type;
    EXPECT_EQ(int_type.toString(), result.type->toString());
}

TEST_F(LookupAddressAndTypeByNameTest, multidim_array) {
    // ARRANGE
    const DataType * data_type = dataTypeInator.resolve("int[5][4][3]");
    int var_to_search[5][4][3];

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "[3][2][1]");
    bool status = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search[3][2][1], result.address);
    SpecifiedPrimitiveDataType<int> int_type;
    EXPECT_EQ(int_type.toString(), result.type->toString());
}


TEST_F(LookupAddressAndTypeByNameTest, multidim_array2) {
    // ARRANGE
    const DataType * data_type = dataTypeInator.resolve("int[5][4][3]");
    int var_to_search[5][4][3];

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "[3][2]");
    bool status = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search[3][2], result.address);
    const DataType * expected_data_type = dataTypeInator.resolve("int[3]");
    EXPECT_EQ(expected_data_type->toString(), result.type->toString());
}

TEST_F(LookupAddressAndTypeByNameTest, composite_array) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * data_type = dataTypeInator.resolve("ClassTwo[5]");
    ClassTwo var_to_search[5];

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "[3].c1.b");
    bool status = visitor.go(data_type);
    
    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search[3].c1.b, result.address);

    DoubleDataType double_type;
    EXPECT_EQ(double_type.toString(), result.type->toString());
}

TEST_F(LookupAddressAndTypeByNameTest, vector_elem) {
    // ARRANGE
    std::vector<int> var_to_search({1, 2, 3, 4, 5});

    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");
    type->validate(&dataTypeInator);

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "[3]");
    bool status = visitor.go(type);
    
    // ASSERT
    ASSERT_TRUE(status);
    auto result = visitor.getResult();

    EXPECT_EQ(&var_to_search[3], result.address);

    IntDataType result_type;
    EXPECT_EQ(result_type.toString(), result.type->toString());
}

TEST_F(LookupAddressAndTypeByNameTest, vector_elem_out_of_bounds) {
    // ARRANGE
    std::vector<int> var_to_search({1, 2, 3, 4, 5});

    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");
    type->validate(&dataTypeInator);

    // ACT
    LookupAddressAndTypeByNameVisitor visitor(&var_to_search, "[100]");
    bool status = visitor.go(type);
    
    // ASSERT
    ASSERT_FALSE(status);
}

}