#include "Algorithm/DataTypeAlgorithm.hpp"
#include "Type/SpecifiedPrimitiveDataType.hpp"
#include "Type/SpecifiedSequenceDataType.hpp"
#include "Type/CompositeDataType.hpp"

#include "DataTypeInator.hpp"
#include "DataTypeTestSupport.hpp"

#include <gtest/gtest.h>

class ResizeSequenceTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;

    ResizeSequenceTest() {}
    ~ResizeSequenceTest() {}
};

TEST_F( ResizeSequenceTest , bare_sequence ) {

    // ARRANGE    
    SequenceDataType * type = new SpecifiedSequenceDataType<std::vector<int>>(  "std::vector<int>");
    type->validate(&dataTypeInator);

    std::vector<int> vec({1, 2, 3, 4, 5});

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &vec, "size", 10);

    // ASSERT
    ASSERT_TRUE(result);
    EXPECT_EQ(10, vec.size());
}

class VecClass {
    public:
    std::vector<int> v;
};

void addVecClassToDataTypeInator(DataTypeInator& dataTypeInator) {

    SequenceDataType * vecType = new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>");
    vecType->validate(&dataTypeInator);
    dataTypeInator.addToDictionary("std::vector<int>", vecType);

    CompositeDataType * vecClassSpec =
        new CompositeDataType( "VecClass", sizeof(VecClass), &construct<VecClass>, &destruct<VecClass>);
        vecClassSpec->addRegularMember( "v", offsetof(VecClass, v), "std::vector<int>");

    dataTypeInator.addToDictionary("VecClass", vecClassSpec);

    vecClassSpec->validate(&dataTypeInator);
}

TEST_F( ResizeSequenceTest , sequence_nested_in_class ) {

    // ARRANGE    
    addVecClassToDataTypeInator(dataTypeInator);

    VecClass var[5];
    const DataType * type = dataTypeInator.resolve("VecClass[4]");
    ASSERT_TRUE(type != NULL);

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "[3].v.size", 10);

    // ASSERT
    ASSERT_TRUE(result);
    EXPECT_EQ(10, var[3].v.size());
}

TEST_F( ResizeSequenceTest , nested_sequence ) {

    // ARRANGE    
    dataTypeInator.addToDictionary("std::vector<int>", new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>"));
    dataTypeInator.addToDictionary("std::vector<std::vector<int>>", new SpecifiedSequenceDataType<std::vector<std::vector<int>>>("std::vector<std::vector<int>>"));
    dataTypeInator.validateDictionary();

    std::vector<std::vector<int>> var (5);
    const DataType * type = dataTypeInator.resolve("std::vector<std::vector<int>>");
    ASSERT_TRUE(type != NULL);

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "[3].size", 10);

    // ASSERT
    ASSERT_TRUE(result);
    EXPECT_EQ(5, var.size());
    EXPECT_EQ(0, var[0].size());
    EXPECT_EQ(10, var[3].size());
}


// TEST_F(ResizeSequenceTest, primitive) {
//     // ARRANGE
//     DataType * type = new DoubleDataType();

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(false, result);

    
//     delete type;
// }

// TEST_F(ResizeSequenceTest, array) {
//     // ARRANGE
//     const DataType * type = dataTypeInator.resolve("double[5]");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(false, result);

//     delete type;
// }

// TEST_F(ResizeSequenceTest, multidim_array) {
//     // ARRANGE
//     const DataType * type = dataTypeInator.resolve("long[5][4][2][1]");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(false, result);

//     delete type;
// }

// TEST_F(ResizeSequenceTest, string) {
//     // ARRANGE
//     const DataType * type = dataTypeInator.resolve("std::string");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(false, result);

//     delete type;
// }

// TEST_F(ResizeSequenceTest, bare_pointer) {
//     // ARRANGE
//     const DataType * type = dataTypeInator.resolve("int *");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(true, result);

//     delete type;
// }

// TEST_F(ResizeSequenceTest, void_pointer) {
//     // ARRANGE
//     const DataType * type = dataTypeInator.resolve("void *");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(true, result);

//     delete type;
// }

// TEST_F(ResizeSequenceTest, array_of_pointers) {
//     // ARRANGE
//     const DataType * type = dataTypeInator.resolve("int *[6]");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(true, result);

//     delete type;
// }

// TEST_F(ResizeSequenceTest, pointer_to_array) {
//     // ARRANGE
//     const DataType * type = dataTypeInator.resolve("int (*)[6]");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(true, result);

//     delete type;
// }

// TEST_F(ResizeSequenceTest, class_with_no_pointers) {
//     // ARRANGE
//     addPointerTestClassesToDictionary(&dataTypeInator);
//     const DataType * type = dataTypeInator.resolve("ClassWithNoPointers");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(false, result);

//     delete type;
// }

// TEST_F(ResizeSequenceTest, class_with_pointers) {
//     // ARRANGE
//     addPointerTestClassesToDictionary(&dataTypeInator);

//     const DataType * type = dataTypeInator.resolve("ClassWithPointer");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(true, result);

//     delete type;
// }

// TEST_F(ResizeSequenceTest, class_with_nested_pointers) {
//     // ARRANGE
//     addPointerTestClassesToDictionary(&dataTypeInator);

//     const DataType * type = dataTypeInator.resolve("ClassWithNestedClasses");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(true, result);

//     delete type;
// }


// TEST_F(ResizeSequenceTest, enum) {
//     // ARRANGE
//     EnumDictionary enumDictionary;
//     addDayOfWeekEnumToTypeDictionary(&dataTypeInator, &enumDictionary);

//     const DataType * type = dataTypeInator.resolve("DayOfWeek");

//     // ACT
//     bool result = DataTypeAlgorithm::resizeSequence(type);

//     // ASSERT
//     ASSERT_EQ(false, result);

//     delete type;
// }