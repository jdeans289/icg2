#include <gtest/gtest.h>
#include <stddef.h>
#include <iostream>
#include "DataTypeInator.hpp"
#include "Type/ArrayDataType.hpp"
#include "Value/FloatingPointValue.hpp"
#include "Value/IntegerValue.hpp"

// Framework
class ArrayDataTypeTest : public ::testing::Test {
    protected:
    DataTypeInator *dataTypeInator;
    ArrayDataTypeTest() { dataTypeInator = new DataTypeInator; }
    ~ArrayDataTypeTest() { delete dataTypeInator; }
    void SetUp() {}
    void TearDown() {}
};

/* ================================================================================
                                         Test Cases
   ================================================================================
*/

TEST_F( ArrayDataTypeTest , constructor_exception ) {

    std::cout << "===== Expecting exception and corresponding message. =====" << std::endl;

    // Attempt to create type an ArrayDataType with 0 dimensions.
    // This attempt should throw an exception.

    ArrayDataType * arrayTypeSpec;
    bool constructor_result = true;
    try {
        arrayTypeSpec = new ArrayDataType( "double", 0);
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        constructor_result = false;
    }

    // Verify that an exception was raised.
    EXPECT_EQ(false, constructor_result);

    // Clean up.
    if (constructor_result) {
        delete arrayTypeSpec;
    }
}

TEST_F( ArrayDataTypeTest, copy_constructor ) {

    try {
    // Create a ArrayDataType.
    ArrayDataType* orig = new ArrayDataType(  "float", 7);

    std::cout << orig->toString();

    // Duplicate it.
    ArrayDataType* copy = new ArrayDataType( *(const ArrayDataType*)orig );

    // Delete the original, so we know the copy is independent.
    delete orig;

    // Verify that the duplicate ArrayDataType is what we expect.
    std::stringstream ss;
    ss << copy->toString();
    EXPECT_EQ("float[7]", ss.str());

    // Clean up.
    delete copy;

    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
    }

}

TEST_F( ArrayDataTypeTest , assignment_operator ) {

    // Create an ArrayDataType.
    ArrayDataType orig = ArrayDataType(  "long", 7);

    std::cout << orig.toString();

    // Assign it to another ArrayDataType.
    ArrayDataType copy = orig;

    // Verify that the assigned ArrayDataType is what we expect.
    std::stringstream ss;
    ss << copy.toString();
    EXPECT_EQ("long[7]", ss.str());
}

TEST_F( ArrayDataTypeTest , clone ) {

    // Create a ArrayDataType.
    // int cdims[] = {3,4,5,-1,-1};
    ArrayDataType* orig = new ArrayDataType( "int**[4][5]",3);
    // Clone it.
    DataType * copy = orig->clone();

    // Delete the original, so we know the copy is independent.
    delete orig;

    // Verify that the cloned ArrayDataType is what we expect.
    std::stringstream ss;
    ss << copy->toString();
    EXPECT_EQ("int**[3][4][5]", ss.str());

    // Clean up.
    delete copy;
}

TEST_F( ArrayDataTypeTest , getSize ) {

    // Create a PointerDataType.
    ArrayDataType* ptrTypeSpec = new ArrayDataType(  "long[4][5]", 3);
    ptrTypeSpec->validate(dataTypeInator);

    // Verify that getSize returns the size of the array.
    EXPECT_EQ( sizeof(long)*60 , ptrTypeSpec->getSize());
}

TEST_F( ArrayDataTypeTest, validate_1 ) {

    // Create a ArrayDataType.
    ArrayDataType* arrayTypeSpec = new ArrayDataType(  "double", 4);

    // Validate the Type.
    bool validation_result = arrayTypeSpec->validate(dataTypeInator);

    // Verify that validation succeeded.
    ASSERT_EQ(true, validation_result);

}

TEST_F( ArrayDataTypeTest , validate_2 ) {

    std::cout << "===== Expecting error message. =====" << std::endl;

    // Create an ArrayDataType.
    ArrayDataType* arrayTypeSpec = new ArrayDataType(  "Undefined_Type", 4);

    // Validate the Type.
    bool validation_result = arrayTypeSpec->validate(dataTypeInator);

    // Verify that validation failed.
    ASSERT_EQ(false, validation_result);
}

TEST_F( ArrayDataTypeTest , getSubType_1 ) {

    // Create type: double [2][3]
    ArrayDataType * arrayTypeSpec;
    bool constructor_result = true;
    try {
        arrayTypeSpec = new ArrayDataType(  "double[3]", 2);
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        constructor_result = false;
    }
    ASSERT_EQ(true, constructor_result);

    // Validate the Type.
    bool validation_result = arrayTypeSpec->validate(dataTypeInator);
    ASSERT_EQ(true, validation_result);

    // Get the subType.
    const DataType * elementTypeSpec;
    elementTypeSpec = arrayTypeSpec->getSubType();
    ASSERT_NE((const DataType *)NULL, elementTypeSpec);

    // Make sure it's: double[3].
    std::stringstream ss;
    ss << elementTypeSpec->toString();
    EXPECT_EQ("double[3]", ss.str());
}

TEST_F( ArrayDataTypeTest , getSubType_2 ) {

    // Create type: double[2]
    ArrayDataType * arrayTypeSpec;
    bool constructor_result = true;
    try {
        arrayTypeSpec = new ArrayDataType(  "double", 2);
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        constructor_result = false;
    }
    ASSERT_EQ(true, constructor_result);

    // Validate the Type.
    bool validation_result = arrayTypeSpec->validate(dataTypeInator);
    ASSERT_EQ(true, validation_result);

    // Get the SubType.
    const DataType * elementTypeSpec;
    elementTypeSpec = arrayTypeSpec->getSubType();
    ASSERT_NE((const DataType *)NULL, elementTypeSpec);

    // Make sure it's: double
    std::stringstream ss;
    ss << elementTypeSpec->toString();
    EXPECT_EQ("double", ss.str());
}
