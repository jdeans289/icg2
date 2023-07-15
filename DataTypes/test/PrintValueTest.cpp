#include "Type/EnumDictionary.hpp"
#include "Type/SpecifiedPrimitiveDataType.hpp"
#include "Type/NormalStructMember.hpp"
#include "DataTypeTestSupport.hpp"
#include "Type/Types.hpp"

#include "gtest/gtest.h"

#include "Algorithm/DataTypeAlgorithm.hpp"


class PrintValueTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    PrintValueTest() {

    }

    ~PrintValueTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

namespace PrintValue {

TEST_F(PrintValueTest, integer) {
    // ARRANGE
    const DataType * type = new IntDataType;
    int val_to_print = 42;
    std::stringstream ss;

    // ACT
    DataTypeAlgorithm::printValue(type, ss, &val_to_print);

    // ASSERT
    std::string expected = "42";
    ASSERT_EQ(expected, ss.str());


    // cleanup
    delete type;
}

TEST_F(PrintValueTest, floating_point) {
    // ARRANGE
    const DataType * type = new DoubleDataType;
    double val_to_print = -42.42;
    std::stringstream ss;

    // ACT
    DataTypeAlgorithm::printValue(type, ss, &val_to_print);

    // ASSERT
    std::string expected = "-42.42";
    ASSERT_EQ(expected, ss.str());

    // cleanup
    delete type;
}


TEST_F( PrintValueTest , pointer ) {
    // ARRANGE
    const DataType * type = dataTypeInator.resolve("double*");
    double * val_to_print = (double*)0xDEADBEEF;
    std::stringstream ss;

    // ACT
    DataTypeAlgorithm::printValue(type, ss, &val_to_print);
    
    // ASSERT
    std::string expected = "0xdeadbeef";
    ASSERT_EQ(expected, ss.str());

    // cleanup
    delete type;
}

TEST_F( PrintValueTest, array ) {

    // ARRANGE
    double val_to_print[2][3][4] = {{{1.2, 2.3, 3.4, 4.5}, {5.6, 6.7, 7.8, 8.9}, {9.0, 0.1, 1.2, 2.3}},
                                    {{3.4, 4.5, 5.6, 6.7}, {7.8, 8.9, 9.0, 0.1}, {1.2, 2.3, 3.4, 4.5}}};

    const DataType * type = dataTypeInator.resolve("double[2][3][4]");
    std::stringstream ss;

    // ACT
    DataTypeAlgorithm::printValue(type, ss, &val_to_print);

    // ASSERT
    std::string expected = "{{{1.2,2.3,3.4,4.5},"
        "{5.6,6.7,7.8,8.9},"
        "{9,0.1,1.2,2.3}},"
        "{{3.4,4.5,5.6,6.7},"
        "{7.8,8.9,9,0.1},"
        "{1.2,2.3,3.4,4.5}}}"
        ;
    ASSERT_EQ(expected, ss.str());


    // cleanup
    delete type;
}


TEST_F(PrintValueTest, enumerated ) {
    // ARRANGE
    addDayOfWeekEnumToTypeDictionary(&dataTypeInator, &enumDictionary);
    const DataType* type = dataTypeInator.resolve("DayOfWeek");
    DayOfWeek val_to_print = Monday;
    std::stringstream ss;

    // ACT
    DataTypeAlgorithm::printValue(type, ss, &val_to_print);

    // ASSERT
    std::string expected = "Monday";
    ASSERT_EQ(expected, ss.str());


    // cleanup
    delete type;
}


TEST_F(PrintValueTest, composite) {
    // ARRANGE
    ClassTwo val_to_print = {.x = 100, .y = 5.5, .c1 = {.a = 5, .b = 1.5}};

    addClassOneToTypeDictionary( &dataTypeInator );
    addClassTwoToTypeDictionary( &dataTypeInator );
    const DataType* type = dataTypeInator.resolve("ClassTwo");
    std::stringstream ss;

    // ACT
    DataTypeAlgorithm::printValue(type, ss, &val_to_print);

    // ASSERT
    std::string expected = "{100, 5.5, {5, 1.5}}";
    ASSERT_EQ(expected, ss.str());

    // cleanup
    delete type;
}

TEST_F(PrintValueTest, string) {
    // ARRANGE
    std::string val_to_print = "I was not able to light on any map or work giving the exact locality of the Castle Dracula, as there are no maps of this country as yet to compare with our own Ordnance Survey maps; but I found that Bistritz, the post town named by Count Dracula, is a fairly well-known place.";
    const DataType* type = new StringDataType;
    std::stringstream ss;

    // ACT
    DataTypeAlgorithm::printValue(type, ss, &val_to_print);

    // ASSERT
    std::string expected = "\"" + val_to_print + "\"";
    ASSERT_EQ(expected, ss.str());

    // cleanup
    delete type;
}

TEST_F(PrintValueTest, string_class) {
    // ARRANGE
    addClassSixToTypeDictionary(&dataTypeInator);
    ClassSix val_to_print;
    val_to_print.str = "Dr. A Cula";
    val_to_print.char_ptr = (char *) 0xdeadbeef;
    const DataType* type = dataTypeInator.resolve("ClassSix");
    std::stringstream ss;

    // ACT
    DataTypeAlgorithm::printValue(type, ss, &val_to_print);

    // ASSERT
    std::string expected = "{0xdeadbeef, \"Dr. A Cula\"}";
    ASSERT_EQ(expected, ss.str());

    // cleanup
    delete type;
}

TEST_F(PrintValueTest, null_type) {
    // ARRANGE
    const DataType* type = NULL;
    int val_to_print = 5;
    std::stringstream ss;

    try {
        // ACT
        DataTypeAlgorithm::printValue(type, ss, &val_to_print);

        // ASSERT
        FAIL() << "Expected a thrown exception.";

    } catch (std::exception& e) {
        // Just assert that there is an error message
        ASSERT_TRUE(strlen(e.what()) > 0);
    }
}

TEST_F(PrintValueTest, unvalidated_type) {
    // ARRANGE
    const DataType* type = new ArrayDataType(&dataTypeInator, std::string("no type with this name"), 100);
    int val_to_print = 5;
    std::stringstream ss;

    try {
        // ACT
        DataTypeAlgorithm::printValue(type, ss, &val_to_print);

        // ASSERT
        FAIL() << "Expected a thrown exception.";

    } catch (std::exception& e) {
        // Just assert that there is an error message
        ASSERT_TRUE(strlen(e.what()) > 0);
    }
}

}