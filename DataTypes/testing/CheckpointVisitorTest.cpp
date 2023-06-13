#include "EnumDictionary.hh"
#include "MemoryManagement/MemMgr.hh"
#include "Type/PrimitiveDataType.hh"
#include "Algorithm/CheckpointVisitor.hh"

#include "DataTypeTestSupport.hh"


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
    int var_to_checkpoint = 100;

    CheckpointVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    int_data_type.accept(&visitor);

    std::cout << "\n\nResults: " << std::endl;
    auto results = visitor.getResults();
    for (auto result : results) {
        std::cout << result.toString() << std::endl;
    }
}

TEST_F(CheckpointVisitorTest, array) {
    // PrimitiveDataType<int> int_data_type;
    const DataType * data_type = dataTypeInator.resolve("int[5]");
    int var_to_checkpoint[5] = {1, 2, 3, 4, 5};

    CheckpointVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    data_type->accept(&visitor);

    std::cout << "\n\nResults: " << std::endl;
    auto results = visitor.getResults();
    for (auto result : results) {
        std::cout << result.toString() << std::endl;
    }
}

TEST_F(CheckpointVisitorTest, composite1) {
    addClassOneToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassOne");
    ClassOne var_to_checkpoint = {.a = 5, .b = 1.5};

    CheckpointVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    data_type->accept(&visitor);

    std::cout << "\n\nResults: " << std::endl;
    auto results = visitor.getResults();
    for (auto result : results) {
        std::cout << result.toString() << std::endl;
    }
}

TEST_F(CheckpointVisitorTest, composite2) {
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * data_type = dataTypeInator.resolve("ClassTwo");
    ClassTwo var_to_checkpoint = {.x = 100, .y = 5.5, .c1 = {.a = 5, .b = 1.5}};

    CheckpointVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    data_type->accept(&visitor);

    std::cout << "\n\nResults: " << std::endl;
    auto results = visitor.getResults();
    for (auto result : results) {
        std::cout << result.toString() << std::endl;
    }
}

TEST_F(CheckpointVisitorTest, composite3) {
    addClassThreeToTypeDictionary(&dataTypeInator);

    const DataType * data_type = dataTypeInator.resolve("ClassThree");
    ClassThree var_to_checkpoint = {.pos = {5, 6}, .vel = {7, 8}};

    CheckpointVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    data_type->accept(&visitor);

    std::cout << "\n\nResults: " << std::endl;
    auto results = visitor.getResults();
    for (auto result : results) {
        std::cout << result.toString() << std::endl;
    }
}