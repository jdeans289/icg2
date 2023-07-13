#include "Algorithm/DataTypeAlgorithm.hpp"
#include "Type/SpecifiedPrimitiveDataType.hpp"
#include "DataTypeInator.hpp"
#include "DataTypeTestSupport.hpp"

#include <gtest/gtest.h>

class ContainsPointerVisitorTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;

    ContainsPointerVisitorTest() {}
    ~ContainsPointerVisitorTest() {}
};

TEST_F(ContainsPointerVisitorTest, primitive) {
    // ARRANGE
    DataType * type = new DoubleDataType();

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(false, result);

    
    delete type;
}

TEST_F(ContainsPointerVisitorTest, array) {
    // ARRANGE
    const DataType * type = dataTypeInator.resolve("double[5]");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(false, result);

    delete type;
}

TEST_F(ContainsPointerVisitorTest, multidim_array) {
    // ARRANGE
    const DataType * type = dataTypeInator.resolve("long[5][4][2][1]");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(false, result);

    delete type;
}

TEST_F(ContainsPointerVisitorTest, string) {
    // ARRANGE
    const DataType * type = dataTypeInator.resolve("std::string");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(false, result);

    delete type;
}

TEST_F(ContainsPointerVisitorTest, bare_pointer) {
    // ARRANGE
    const DataType * type = dataTypeInator.resolve("int *");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(true, result);

    delete type;
}

TEST_F(ContainsPointerVisitorTest, void_pointer) {
    // ARRANGE
    const DataType * type = dataTypeInator.resolve("void *");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(true, result);

    delete type;
}

TEST_F(ContainsPointerVisitorTest, array_of_pointers) {
    // ARRANGE
    const DataType * type = dataTypeInator.resolve("int *[6]");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(true, result);

    delete type;
}

TEST_F(ContainsPointerVisitorTest, pointer_to_array) {
    // ARRANGE
    const DataType * type = dataTypeInator.resolve("int (*)[6]");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(true, result);

    delete type;
}

TEST_F(ContainsPointerVisitorTest, class_with_no_pointers) {
    // ARRANGE
    addPointerTestClassesToDictionary(&dataTypeInator);
    const DataType * type = dataTypeInator.resolve("ClassWithNoPointers");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(false, result);

    delete type;
}

TEST_F(ContainsPointerVisitorTest, class_with_pointers) {
    // ARRANGE
    addPointerTestClassesToDictionary(&dataTypeInator);

    const DataType * type = dataTypeInator.resolve("ClassWithPointer");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(true, result);

    delete type;
}

TEST_F(ContainsPointerVisitorTest, class_with_nested_pointers) {
    // ARRANGE
    addPointerTestClassesToDictionary(&dataTypeInator);

    const DataType * type = dataTypeInator.resolve("ClassWithNestedClasses");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(true, result);

    delete type;
}


TEST_F(ContainsPointerVisitorTest, enum) {
    // ARRANGE
    EnumDictionary enumDictionary;
    addDayOfWeekEnumToTypeDictionary(&dataTypeInator, &enumDictionary);

    const DataType * type = dataTypeInator.resolve("DayOfWeek");

    // ACT
    bool result = DataTypeAlgorithm::containsPointer(type);

    // ASSERT
    ASSERT_EQ(false, result);

    delete type;
}