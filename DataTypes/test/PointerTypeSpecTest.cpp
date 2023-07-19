#include <gtest/gtest.h>
#include <stddef.h>
#include <iostream>

#include "TypeDictionary.hpp"
#include "Type/PointerDataType.hpp"
#include "Value/PointerValue.hpp"

// Framework
class PointerDataTypeTest : public ::testing::Test {
    protected:
    DataTypeInator *dataTypeInator;
    PointerDataTypeTest() { dataTypeInator = new DataTypeInator; }
    ~PointerDataTypeTest() { delete dataTypeInator; }
    void SetUp() {}
    void TearDown() {}
};

/* ================================================================================
                                         Test Cases
   ================================================================================
*/


TEST_F( PointerDataTypeTest, copy_constructor ) {

    // Create a PointerDataType.
    PointerDataType* orig = new PointerDataType(dataTypeInator, "float**");

    // Duplicate it.
    PointerDataType* copy = new PointerDataType( *(const PointerDataType*)orig );

    // Verify that the duplicate PointerDataType is what we expect.
    std::stringstream ss;
    ss << copy->toString();
    int result = ss.str().compare("float***");
    EXPECT_EQ(0, result);

    // Clean up.
    delete copy;
    delete orig;
}

TEST_F( PointerDataTypeTest , operator_equal ) {

    // Create a PointerDataType.
    PointerDataType* orig = new PointerDataType(dataTypeInator, "long*");

    // Assign it to another PointerDataType.
    PointerDataType copy = *(const PointerDataType*)orig;

    // Verify that the assigned PointerDataType is what we expect.
    std::stringstream ss;
    ss << copy.toString();
    int result = ss.str().compare("long**");
    EXPECT_EQ(0, result);

    // Clean up.
    delete orig;
}

TEST_F( PointerDataTypeTest , clone ) {

    // Create a PointerDataType.
    PointerDataType* orig = new PointerDataType(dataTypeInator, "int*");

    // Clone it.
    DataType * copy = orig->clone();

    // Verify that the cloned PointerDataType is what we expect.
    std::stringstream ss;
    ss << copy->toString();
    int result = ss.str().compare("int**");
    EXPECT_EQ(0, result);

    // Clean up.
    delete orig;
    delete copy;
}

TEST_F( PointerDataTypeTest , getSize ) {

    // Create a PointerDataType.
    PointerDataType* ptrTypeSpec = new PointerDataType(dataTypeInator, "long**");

    // Verify that getSize returns the size of a pointer.
    EXPECT_EQ( sizeof(void*), ptrTypeSpec->getSize());
}

// TEST_F( PointerDataTypeTest , assignValue ) {

//     double d = 1.2345;
//     double * d_ptr;

//     // Create a PointerDataType.
//     PointerDataType* ptrTypeSpec = new PointerDataType(dataTypeInator, "double");

//     PointerValue * ptrValue = new PointerValue(&d);
//     ptrTypeSpec->assignValue(&d_ptr, ptrValue);

//     EXPECT_EQ(1.2345, *d_ptr);
// }

// TEST_F( PointerDataTypeTest , getValue ) {

//     // ARRANGE
//     double * d_ptr = (double*)0x12345678;

//     // Create a PointerDataType.
//     PointerDataType* ptrTypeSpec = new PointerDataType(dataTypeInator, "double");

//     // ACT
//     Value * value = ptrTypeSpec->getValue(&d_ptr);

//     // ASSERT
//     PointerValue * ptrValue = dynamic_cast<PointerValue *> (value);
//     ASSERT_TRUE(ptrValue != NULL);
//     ASSERT_EQ((double*)0x12345678, ptrValue->getPointer());
// }

TEST_F( PointerDataTypeTest , validate_1 ) {

    // Create a PointerDataType.
    PointerDataType* ptrTypeSpec = new PointerDataType(dataTypeInator, "double");

    // Validate the Type.
    bool validation_result = ptrTypeSpec->validate();

    // Verify that validation succeeded.
    ASSERT_EQ(true, validation_result);

}

TEST_F( PointerDataTypeTest , validate_2 ) {

    std::cout << "===== Expecting an error message about an undefined type. =====" << std::endl;

    // Create a PointerDataType.
    PointerDataType* ptrTypeSpec = new PointerDataType(dataTypeInator, "Undefined_Type");

    // Validate the Type.
    bool validation_result = ptrTypeSpec->validate();

    // Verify that validation failed.
    ASSERT_EQ(false, validation_result);

}

TEST_F( PointerDataTypeTest , getDereferencedType_1 ) {

    // Create type: double**
    PointerDataType * ptrTypeSpec;
    bool constructor_result = true;
    try {
        ptrTypeSpec = new PointerDataType(dataTypeInator, "double*");
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        constructor_result = false;
    }
    ASSERT_EQ(true, constructor_result);

    // Validate the Type.
    bool validation_result = ptrTypeSpec->validate();
    ASSERT_EQ(true, validation_result);

    // Dereference the type.
    const DataType * dereferencedPtrTypeSpec;
    bool  dereference_result = true;
    try {
        dereferencedPtrTypeSpec = ptrTypeSpec->getSubType();
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        dereference_result = false;
    }
    ASSERT_EQ(true, dereference_result);

    // Result of the dereference should be: double*
    std::stringstream ss;
    ss << dereferencedPtrTypeSpec->toString();
    int comparision_result = ss.str().compare("double*");

    EXPECT_EQ(0, comparision_result);

}

TEST_F( PointerDataTypeTest , getDereferencedType_2 ) {

    // Create type: double*
    PointerDataType * ptrTypeSpec;
    bool constructor_result = true;
    try {
        ptrTypeSpec = new PointerDataType(dataTypeInator, "double");
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        constructor_result = false;
    }
    ASSERT_EQ(true, constructor_result);

    // Validate the Type.
    bool validation_result = ptrTypeSpec->validate();
    ASSERT_EQ(true, validation_result);

    // Dereference the type.
    const DataType * dereferencedPtrTypeSpec;
    bool  dereference_result = true;
    try {
        dereferencedPtrTypeSpec = ptrTypeSpec->getSubType();
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        dereference_result = false;
    }
    ASSERT_EQ(true, dereference_result);

    // Result of the dereference should be: double
    std::stringstream ss;
    ss << dereferencedPtrTypeSpec->toString();
    int comparision_result = ss.str().compare("double");

    EXPECT_EQ(0, comparision_result);
}
