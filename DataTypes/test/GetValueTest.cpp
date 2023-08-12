#include "Type/EnumDictionary.hpp"
#include "Type/NormalStructMember.hpp"
#include "DataTypeTestSupport.hpp"
#include "Type/AllTypes.hpp"
#include "Value/PointerValue.hpp"
#include "Value/IntegerValue.hpp"
#include "Value/StringValue.hpp"

#include "gtest/gtest.h"

#include "Algorithm/DataTypeAlgorithm.hpp"


class GetValueTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    GetValueTest() {

    }

    ~GetValueTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

namespace GetValue {


TEST_F(GetValueTest, integer) {
    // ARRANGE
    IntDataType type;
    int var_to_get = 42;

    // ACT
    Value * val = DataTypeAlgorithm::getValue(&type, &var_to_get);

    // ASSERT
    IntegerValue * int_val = dynamic_cast<IntegerValue *> (val);
    ASSERT_TRUE(int_val != NULL);
    ASSERT_EQ(42, int_val->getIntegerValue());

    // cleanup
    delete val;
}

TEST_F(GetValueTest, floating_point) {
    // ARRANGE
    DoubleDataType type;
    double var_to_get = 42.42;

    // ACT
    Value * val = DataTypeAlgorithm::getValue(&type, &var_to_get);

    // ASSERT
    FloatingPointValue * float_val = dynamic_cast<FloatingPointValue *> (val);
    ASSERT_TRUE(float_val != NULL);
    ASSERT_EQ(42.42, float_val->getFloatingPointValue());

    // cleanup
    delete val;
}

TEST_F( GetValueTest , pointer ) {

    // ARRANGE
    const DataType * type = dataTypeInator.resolve("double*");
    double * var_to_get = (double *) 0xc0ffee;

    // ACT
    Value * val = DataTypeAlgorithm::getValue(type, &var_to_get);

    // ASSERT
    PointerValue * pointer_val = dynamic_cast<PointerValue *> (val);
    ASSERT_TRUE(pointer_val != NULL);
    ASSERT_EQ((void *) 0xc0ffee, pointer_val->getPointer());

    // cleanup
    delete val;
    delete type;
}

TEST_F(GetValueTest, string) {

    // ARRANGE
    StringDataType type;
    std::string expected_value = "I was not able to light on any map or work giving the exact locality of the Castle Dracula, as there are no maps of this country as yet to compare with our own Ordnance Survey maps; but I found that Bistritz, the post town named by Count Dracula, is a fairly well-known place.";

    std::string var_to_get = expected_value;

    // ACT
    Value * val = DataTypeAlgorithm::getValue(&type, &var_to_get);

    // ASSERT
    StringValue * str_val = dynamic_cast<StringValue *> (val);
    ASSERT_TRUE(str_val != NULL);
    ASSERT_EQ(expected_value, str_val->getRawString());

    // cleanup
    delete val;
}

TEST_F(GetValueTest, enumerated) {
    // ARRANGE
    addDayOfWeekEnumToTypeDictionary(&dataTypeInator, &enumDictionary);
    const DataType* type = dataTypeInator.resolve("DayOfWeek");
    DayOfWeek var_to_get = Friday;

    // ACT
    Value * val = DataTypeAlgorithm::getValue(type, &var_to_get);

    // ASSERT
    IntegerValue * int_val = dynamic_cast<IntegerValue *> (val);
    ASSERT_TRUE(int_val != NULL);
    ASSERT_EQ((int) Friday, int_val->getIntegerValue());

    // cleanup
    delete val;
    delete type;
}


TEST_F(GetValueTest, array) {
    // ARRANGE
    const DataType * type = dataTypeInator.resolve("double[5]");
    double var_to_get[5] = {1, 2, 3, 4, 5};

    // ACT
    Value * val = DataTypeAlgorithm::getValue(type, &var_to_get);

    // ASSERT
    ASSERT_EQ(NULL, val);
    delete type;
}

class Foo {};

TEST_F(GetValueTest, composite) {
    // ARRANGE
    CompositeDataType type( "Foo", sizeof(Foo), NULL, NULL);
    type.validate(&dataTypeInator);

    double var_to_get[5] = {1, 2, 3, 4, 5};

    // ACT
    Value * val = DataTypeAlgorithm::getValue(&type, &var_to_get);

    // ASSERT
    ASSERT_EQ(NULL, val);
}


}