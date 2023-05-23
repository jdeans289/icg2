#include <gtest/gtest.h>
#include <stddef.h>
#include "DataTypeInator.hh"
#include "PrimitiveDataType.hh"
#include "PointerDataType.hh"
#include "ArrayDataType.hh"

// Framework
class DataTypeInatorTest : public ::testing::Test {
    protected:
    DataTypeInator *dataTypeInator;

    DataTypeInatorTest() { dataTypeInator = new DataTypeInator; }
    ~DataTypeInatorTest() { delete dataTypeInator; }
    void SetUp() {}
    void TearDown() {}
};

TEST_F(DataTypeInatorTest, basic) {
    // ARRANGE
    
    // ACT
    const DataType * result = dataTypeInator->resolve("int");

    // ASSERT
    const PrimitiveDataType<int> * casted_result = dynamic_cast<const PrimitiveDataType<int> *> (result);
    ASSERT_TRUE(casted_result != NULL);
}


TEST_F(DataTypeInatorTest, Array) {
    // ARRANGE
    
    // ACT
    DataType * result = const_cast<DataType *>(dataTypeInator->resolve("int[5]"));
    result->validate();

    // ASSERT
    const ArrayDataType * array_result = dynamic_cast<const ArrayDataType*> (result);
    ASSERT_TRUE(array_result != NULL);
    ASSERT_EQ(array_result->getElementCount(), 5);

    const DataType * subtype_result = array_result->getSubType();
    // std::cout << "Subtype: " << subtype_result->toString() << std::endl;
    const PrimitiveDataType<int> * subtype_casted_result = dynamic_cast<const PrimitiveDataType<int> *> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

TEST_F(DataTypeInatorTest, MultidimArray) {
    // ARRANGE
    
    // ACT
    DataType * result = const_cast<DataType *>(dataTypeInator->resolve("int[5][4]"));
    // result->validate();

    // ASSERT

    // Type is Array with element count of 5
    const ArrayDataType * array_result = dynamic_cast<const ArrayDataType*> (result);
    ASSERT_TRUE(array_result != NULL);
    ASSERT_EQ(array_result->getElementCount(), 5);

    // Subtype is array with element count 4
    const DataType * subtype_result = array_result->getSubType();
    array_result = dynamic_cast<const ArrayDataType*> (subtype_result);
    ASSERT_TRUE(array_result != NULL);
    ASSERT_EQ(array_result->getElementCount(), 4);

    // Subtype's subtype is int
    subtype_result = array_result->getSubType();
    const PrimitiveDataType<int> * subtype_casted_result = dynamic_cast<const PrimitiveDataType<int> *> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

TEST_F(DataTypeInatorTest, Pointer) {
    // ARRANGE
    
    // ACT
    DataType * result = const_cast<DataType *>(dataTypeInator->resolve("int *"));

    // ASSERT

    // Type is a pointer
    const PointerDataType * pointer_result = dynamic_cast<const PointerDataType*> (result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype is int
    const DataType * subtype_result = pointer_result->getSubType();
    const PrimitiveDataType<int> * subtype_casted_result = dynamic_cast<const PrimitiveDataType<int> *> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

TEST_F(DataTypeInatorTest, PointerPointer) {
    // ARRANGE
    
    // ACT
    DataType * result = const_cast<DataType *>(dataTypeInator->resolve("int **"));

    // ASSERT

    // Type is a pointer
    const PointerDataType * pointer_result = dynamic_cast<const PointerDataType*> (result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype is pointer
    const DataType * subtype_result = pointer_result->getSubType();
    pointer_result = dynamic_cast<const PointerDataType *> (subtype_result);
    ASSERT_TRUE(pointer_result != NULL);;

    // Subtype's subtype is int
    subtype_result = pointer_result->getSubType();
    const PrimitiveDataType<int> * subtype_casted_result = dynamic_cast<const PrimitiveDataType<int> *> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

TEST_F(DataTypeInatorTest, ArrayPointerPointer) {
    // ARRANGE
    
    // ACT
    DataType * result = const_cast<DataType *>(dataTypeInator->resolve("int (**)[5]"));

    // ASSERT

    // Type is pointer
    const PointerDataType * pointer_result = dynamic_cast<const PointerDataType *> (result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype is pointer
    const DataType * subtype_result = pointer_result->getSubType();
    pointer_result = dynamic_cast<const PointerDataType *> (subtype_result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype's subtype is an array[5]
    subtype_result = pointer_result->getSubType();
    const ArrayDataType * array_result = dynamic_cast<const ArrayDataType*> (subtype_result);
    ASSERT_TRUE(array_result != NULL);
    ASSERT_EQ(array_result->getElementCount(), 5);

    // Subtype's subtype's subtype is int
    subtype_result = array_result->getSubType();
    const PrimitiveDataType<int> * subtype_casted_result = dynamic_cast<const PrimitiveDataType<int> *> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

TEST_F(DataTypeInatorTest, PointerPointerArray) {
    // ARRANGE
    
    // ACT
    DataType * result = const_cast<DataType *>(dataTypeInator->resolve("int **[5]"));

    // ASSERT

    // Type is array[5]
    const ArrayDataType * array_result = dynamic_cast<const ArrayDataType*> (result);
    ASSERT_TRUE(array_result != NULL);
    ASSERT_EQ(array_result->getElementCount(), 5);

    // Subtype is a pointer
    const DataType * subtype_result = array_result->getSubType();
    const PointerDataType * pointer_result = dynamic_cast<const PointerDataType*> (subtype_result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype's subtype is pointer
    subtype_result = pointer_result->getSubType();
    pointer_result = dynamic_cast<const PointerDataType *> (subtype_result);
    ASSERT_TRUE(pointer_result != NULL);

    // Subtype's subtype's subtype is int
    subtype_result = pointer_result->getSubType();
    const PrimitiveDataType<int> * subtype_casted_result = dynamic_cast<const PrimitiveDataType<int> *> (subtype_result);
    ASSERT_TRUE(subtype_casted_result != NULL);
}

