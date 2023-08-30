#include "Algorithm/DataTypeAlgorithm.hpp"
#include "Type/AllTypes.hpp"

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
    std::shared_ptr<SequenceDataType> type = std::make_shared<SpecifiedSequenceDataType<std::vector<int>>>("std::vector<int>");
    type->validate(&dataTypeInator);

    std::vector<int> vec({1, 2, 3, 4, 5});

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &vec, "size", 10);

    // ASSERT
    ASSERT_TRUE(result);
    EXPECT_EQ(10, vec.size());
}

TEST_F( ResizeSequenceTest , sequence_nested_in_class ) {

    // ARRANGE    
    dataTypeInator.addToDictionary("std::vector<int>", new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>"));
    dataTypeInator.addToDictionary("VecClass", new SpecifiedCompositeType<VecClass>("VecClass"));
    dataTypeInator.validateDictionary();

    VecClass var[5];
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("VecClass[4]");
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
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("std::vector<std::vector<int>>");
    ASSERT_TRUE(type != NULL);

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "[3].size", 10);

    // ASSERT
    ASSERT_TRUE(result);
    EXPECT_EQ(5, var.size());
    EXPECT_EQ(0, var[0].size());
    EXPECT_EQ(10, var[3].size());
}

class VecClass_static {
    public:
    static std::vector<int> v;
};

std::vector<int> VecClass_static::v;

template <>
class SpecifiedCompositeType<VecClass_static> : public CompositeDataType {
    public:
    SpecifiedCompositeType(std::string name) : CompositeDataType(name, sizeof(VecClass_static), &construct_composite<VecClass_static>, &destruct_composite<VecClass_static>) {}

    MemberMap& getMemberMap () override {
        using type_to_add = VecClass_static;

        static MemberMap member_map = {
            {"v", StructMember("v", "std::vector<int>", (long) &VecClass_static::v, StructMember::STATIC)},
        };
        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<VecClass_static>*> (this))->getMemberMap();
    }
};

TEST_F( ResizeSequenceTest , static_sequence_class ) {
    // ARRANGE    
    dataTypeInator.addToDictionary("std::vector<int>", new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>"));
    dataTypeInator.addToDictionary("VecClass_static", new SpecifiedCompositeType<VecClass_static>("VecClass_static"));
    dataTypeInator.validateDictionary();

    VecClass_static var;
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("VecClass_static");
    ASSERT_TRUE(type != NULL);

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "v.size", 10);

    // ASSERT
    ASSERT_TRUE(result);
    EXPECT_EQ(10, var.v.size());
}

TEST_F(ResizeSequenceTest, primitive) {
    // ARRANGE
    std::shared_ptr<DataType> type = std::make_shared<DoubleDataType>();
    int var;

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}

TEST_F(ResizeSequenceTest, string) {
    // ARRANGE
    std::shared_ptr<DataType> type = std::make_shared<StringDataType>();
    std::string var;

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "size", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}

TEST_F(ResizeSequenceTest, array) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double[5]");
    double var[5];


    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "[2].size", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}

TEST_F(ResizeSequenceTest, array_bad_index) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double[5]");
    double var[5];


    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "not_an_index", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}

TEST_F(ResizeSequenceTest, array_out_of_bounds) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("double[5]");
    double var[5];


    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "[100]", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}


TEST_F(ResizeSequenceTest, bare_pointer) {
    // ARRANGE
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("int *");
    int * var;

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}

TEST_F(ResizeSequenceTest, enum) {
    // ARRANGE
    EnumDictionary enumDictionary;
    addDayOfWeekEnumToTypeDictionary(&dataTypeInator, &enumDictionary);
    DayOfWeek var = Monday;
    std::shared_ptr<const DataType> type = dataTypeInator.resolve("DayOfWeek");

    // ACT
    bool result = DataTypeAlgorithm::resizeSequence(type, &var, "", 10);

    // ASSERT
    ASSERT_EQ(false, result);


}