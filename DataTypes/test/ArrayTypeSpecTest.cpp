#include <gtest/gtest.h>
#include <stddef.h>
#include <iostream>
#include "DataTypeInator.hpp"
#include "Type/ArrayDataType.hpp"
#include "Value/ArrayValue.hpp"
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
        arrayTypeSpec = new ArrayDataType(dataTypeInator, "double", 0);
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
    ArrayDataType* orig = new ArrayDataType( dataTypeInator, "float", 7);

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
    ArrayDataType * orig = new ArrayDataType( dataTypeInator, "long", 7);

    std::cout << orig->toString();

    // Assign it to another ArrayDataType.
    ArrayDataType copy = *(const ArrayDataType*)orig;

    // Verify that the assigned ArrayDataType is what we expect.
    std::stringstream ss;
    ss << copy.toString();
    EXPECT_EQ("long[7]", ss.str());

    // Clean up.
    delete orig;
}

TEST_F( ArrayDataTypeTest , clone ) {

    // Create a ArrayDataType.
    // int cdims[] = {3,4,5,-1,-1};
    ArrayDataType* orig = new ArrayDataType(dataTypeInator, "int**[4][5]",3);
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
    ArrayDataType* ptrTypeSpec = new ArrayDataType( dataTypeInator, "long[4][5]", 3);
    ptrTypeSpec->validate();

    // Verify that getSize returns the size of the array.
    EXPECT_EQ( sizeof(long)*60 , ptrTypeSpec->getSize());
}

TEST_F( ArrayDataTypeTest , assignValue ) {

    double A[2][3][4];

    // Create a PointerDataType.
    // int cdims[] = {2,3,4};
    ArrayDataType * arrayTypeSpec = new ArrayDataType( dataTypeInator, "double var[3][4]", 2);

    arrayTypeSpec->validate(); // TODO: Test behavior when it isnt validated also.



    ArrayValue * value_0_0 = new  ArrayValue();
    value_0_0->pushElem( new FloatingPointValue(1.2));
    value_0_0->pushElem( new FloatingPointValue(2.3));
    value_0_0->pushElem( new FloatingPointValue(3.4));
    value_0_0->pushElem( new FloatingPointValue(4.5));

    ArrayValue * value_0_1 = new  ArrayValue();
    value_0_1->pushElem( new FloatingPointValue(5.6));
    value_0_1->pushElem( new FloatingPointValue(6.7));
    value_0_1->pushElem( new FloatingPointValue(7.8));
    value_0_1->pushElem( new FloatingPointValue(8.9));

    ArrayValue * value_0_2 = new  ArrayValue();
    value_0_2->pushElem( new FloatingPointValue(9.0));
    value_0_2->pushElem( new FloatingPointValue(0.1));
    value_0_2->pushElem( new FloatingPointValue(1.2));
    value_0_2->pushElem( new FloatingPointValue(2.3));

    ArrayValue * value_1_0 = new  ArrayValue();
    value_1_0->pushElem( new FloatingPointValue(3.4));
    value_1_0->pushElem( new FloatingPointValue(4.5));
    value_1_0->pushElem( new FloatingPointValue(5.6));
    value_1_0->pushElem( new FloatingPointValue(6.7));

    ArrayValue * value_1_1 = new  ArrayValue();
    value_1_1->pushElem( new FloatingPointValue(7.8));
    value_1_1->pushElem( new FloatingPointValue(8.9));
    value_1_1->pushElem( new FloatingPointValue(9.0));
    value_1_1->pushElem( new FloatingPointValue(0.1));

    ArrayValue * value_1_2 = new  ArrayValue();
    value_1_2->pushElem( new FloatingPointValue(1.2));
    value_1_2->pushElem( new FloatingPointValue(2.3));
    value_1_2->pushElem( new FloatingPointValue(3.4));
    value_1_2->pushElem( new FloatingPointValue(4.5));

    ArrayValue * value_0 = new  ArrayValue();
    value_0->pushElem( value_0_0 );
    value_0->pushElem( value_0_1 );
    value_0->pushElem( value_0_2 );

    ArrayValue * value_1 = new  ArrayValue();
    value_1->pushElem( value_1_0 );
    value_1->pushElem( value_1_1 );
    value_1->pushElem( value_1_2 );

    ArrayValue * value = new  ArrayValue();
    value->pushElem( value_0 );
    value->pushElem( value_1 );

    arrayTypeSpec->assignValue(A, value);

    EXPECT_EQ(1.2, A[0][0][0]);
    EXPECT_EQ(2.3, A[0][0][1]);
    EXPECT_EQ(3.4, A[0][0][2]);
    EXPECT_EQ(4.5, A[0][0][3]);

    EXPECT_EQ(5.6, A[0][1][0]);
    EXPECT_EQ(6.7, A[0][1][1]);
    EXPECT_EQ(7.8, A[0][1][2]);
    EXPECT_EQ(8.9, A[0][1][3]);

    EXPECT_EQ(9.0, A[0][2][0]);
    EXPECT_EQ(0.1, A[0][2][1]);
    EXPECT_EQ(1.2, A[0][2][2]);
    EXPECT_EQ(2.3, A[0][2][3]);

    EXPECT_EQ(3.4, A[1][0][0]);
    EXPECT_EQ(4.5, A[1][0][1]);
    EXPECT_EQ(5.6, A[1][0][2]);
    EXPECT_EQ(6.7, A[1][0][3]);

    EXPECT_EQ(7.8, A[1][1][0]);
    EXPECT_EQ(8.9, A[1][1][1]);
    EXPECT_EQ(9.0, A[1][1][2]);
    EXPECT_EQ(0.1, A[1][1][3]);

    EXPECT_EQ(1.2, A[1][2][0]);
    EXPECT_EQ(2.3, A[1][2][1]);
    EXPECT_EQ(3.4, A[1][2][2]);
    EXPECT_EQ(4.5, A[1][2][3]);
}

TEST_F( ArrayDataTypeTest , createInstance_and_assignValue ) {

    // Create a PointerDataType.
    int cdims[] = {2,3,4};
    ArrayDataType * arrayTypeSpec = new ArrayDataType( dataTypeInator, "double[3][4]", 2);

    arrayTypeSpec->validate(); // TODO: Test behavior when it isnt validated also.

    double (*A)[2][3][4] = (double(*)[2][3][4])arrayTypeSpec->createInstance(1);



    ArrayValue * value_0_0 = new  ArrayValue();
    value_0_0->pushElem( new FloatingPointValue(1.2));
    value_0_0->pushElem( new FloatingPointValue(2.3));
    value_0_0->pushElem( new FloatingPointValue(3.4));
    value_0_0->pushElem( new FloatingPointValue(4.5));

    ArrayValue * value_0_1 = new  ArrayValue();
    value_0_1->pushElem( new FloatingPointValue(5.6));
    value_0_1->pushElem( new FloatingPointValue(6.7));
    value_0_1->pushElem( new FloatingPointValue(7.8));
    value_0_1->pushElem( new FloatingPointValue(8.9));

    ArrayValue * value_0_2 = new  ArrayValue();
    value_0_2->pushElem( new FloatingPointValue(9.0));
    value_0_2->pushElem( new FloatingPointValue(0.1));
    value_0_2->pushElem( new FloatingPointValue(1.2));
    value_0_2->pushElem( new FloatingPointValue(2.3));

    ArrayValue * value_1_0 = new  ArrayValue();
    value_1_0->pushElem( new FloatingPointValue(3.4));
    value_1_0->pushElem( new FloatingPointValue(4.5));
    value_1_0->pushElem( new FloatingPointValue(5.6));
    value_1_0->pushElem( new FloatingPointValue(6.7));

    ArrayValue * value_1_1 = new  ArrayValue();
    value_1_1->pushElem( new FloatingPointValue(7.8));
    value_1_1->pushElem( new FloatingPointValue(8.9));
    value_1_1->pushElem( new FloatingPointValue(9.0));
    value_1_1->pushElem( new FloatingPointValue(0.1));

    ArrayValue * value_1_2 = new  ArrayValue();
    value_1_2->pushElem( new FloatingPointValue(1.2));
    value_1_2->pushElem( new FloatingPointValue(2.3));
    value_1_2->pushElem( new FloatingPointValue(3.4));
    value_1_2->pushElem( new FloatingPointValue(4.5));

    ArrayValue * value_0 = new  ArrayValue();
    value_0->pushElem( value_0_0 );
    value_0->pushElem( value_0_1 );
    value_0->pushElem( value_0_2 );

    ArrayValue * value_1 = new  ArrayValue();
    value_1->pushElem( value_1_0 );
    value_1->pushElem( value_1_1 );
    value_1->pushElem( value_1_2 );

    ArrayValue * value = new  ArrayValue();
    value->pushElem( value_0 );
    value->pushElem( value_1 );

    arrayTypeSpec->assignValue(A, value);

    EXPECT_EQ(1.2, (*A)[0][0][0]);
    EXPECT_EQ(2.3, (*A)[0][0][1]);
    EXPECT_EQ(3.4, (*A)[0][0][2]);
    EXPECT_EQ(4.5, (*A)[0][0][3]);

    EXPECT_EQ(5.6, (*A)[0][1][0]);
    EXPECT_EQ(6.7, (*A)[0][1][1]);
    EXPECT_EQ(7.8, (*A)[0][1][2]);
    EXPECT_EQ(8.9, (*A)[0][1][3]);

    EXPECT_EQ(9.0, (*A)[0][2][0]);
    EXPECT_EQ(0.1, (*A)[0][2][1]);
    EXPECT_EQ(1.2, (*A)[0][2][2]);
    EXPECT_EQ(2.3, (*A)[0][2][3]);

    EXPECT_EQ(3.4, (*A)[1][0][0]);
    EXPECT_EQ(4.5, (*A)[1][0][1]);
    EXPECT_EQ(5.6, (*A)[1][0][2]);
    EXPECT_EQ(6.7, (*A)[1][0][3]);

    EXPECT_EQ(7.8, (*A)[1][1][0]);
    EXPECT_EQ(8.9, (*A)[1][1][1]);
    EXPECT_EQ(9.0, (*A)[1][1][2]);
    EXPECT_EQ(0.1, (*A)[1][1][3]);

    EXPECT_EQ(1.2, (*A)[1][2][0]);
    EXPECT_EQ(2.3, (*A)[1][2][1]);
    EXPECT_EQ(3.4, (*A)[1][2][2]);
    EXPECT_EQ(4.5, (*A)[1][2][3]);
}

TEST_F( ArrayDataTypeTest, validate_1 ) {

    // Create a ArrayDataType.
    int cdims[] = {4};
    ArrayDataType* arrayTypeSpec = new ArrayDataType( dataTypeInator, "double", 4);

    // Validate the Type.
    bool validation_result = arrayTypeSpec->validate();

    // Verify that validation succeeded.
    ASSERT_EQ(true, validation_result);

}

TEST_F( ArrayDataTypeTest , validate_2 ) {

    std::cout << "===== Expecting error message. =====" << std::endl;

    // Create an ArrayDataType.
    ArrayDataType* arrayTypeSpec = new ArrayDataType( dataTypeInator, "Undefined_Type", 4);

    // Validate the Type.
    bool validation_result = arrayTypeSpec->validate();

    // Verify that validation failed.
    ASSERT_EQ(false, validation_result);
}

TEST_F( ArrayDataTypeTest , getSubType_1 ) {

    // Create type: double [2][3]
    ArrayDataType * arrayTypeSpec;
    bool constructor_result = true;
    try {
        arrayTypeSpec = new ArrayDataType( dataTypeInator, "double[3]", 2);
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        constructor_result = false;
    }
    ASSERT_EQ(true, constructor_result);

    // Validate the Type.
    bool validation_result = arrayTypeSpec->validate();
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
        arrayTypeSpec = new ArrayDataType( dataTypeInator, "double", 2);
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        constructor_result = false;
    }
    ASSERT_EQ(true, constructor_result);

    // Validate the Type.
    bool validation_result = arrayTypeSpec->validate();
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
