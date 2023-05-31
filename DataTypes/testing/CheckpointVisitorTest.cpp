#include "MemMgr.hh"
// #include "DataTypeInator.hh"
#include "EnumDictionary.hh"

#include "PrimitiveDataType.hh"

#include "DataTypeTestSupport.hh"

#include "CheckpointVisitor.hh"

#include "gtest/gtest.h"

class CheckpointVisitorTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;
    MemMgr memMgr;

    CheckpointVisitorTest() {

    }

    ~CheckpointVisitorTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

TEST_F(CheckpointVisitorTest, basic) {
    PrimitiveDataType<int> int_data_type;

    CheckpointVisitor visitor;
    int_data_type.accept(&visitor);
}

TEST_F(CheckpointVisitorTest, composite) {
    // PrimitiveDataType<int> int_data_type;
    addClassOneToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassOne");

    CheckpointVisitor visitor;
    data_type->accept(&visitor);
}