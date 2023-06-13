#include "EnumDictionary.hh"
#include "MemoryManagement/MemMgr.hh"
#include "Type/PrimitiveDataType.hh"
#include "Type/NormalStructMember.hh"
#include "DataTypeTestSupport.hh"

#include "gtest/gtest.h"

#include "Algorithm/LookupAddressVisitor.hh"


class LookupAddressVisitorTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;
    MemMgr memMgr;

    LookupAddressVisitorTest() {

    }

    ~LookupAddressVisitorTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

TEST_F(LookupAddressVisitorTest, basic) {
    // ARRANGE
    PrimitiveDataType<int> int_data_type;
    int var_to_search = 100;

    int * search_address = &var_to_search;

    LookupAddressVisitor visitor("var_to_search", &var_to_search, search_address);

    // ACT
    bool success = int_data_type.accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult().toString();
    std::string expected = "var_to_search";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupAddressVisitorTest, array) {
    // ARRANGE
    const DataType * data_type = dataTypeInator.resolve("int[5]");
    int var_to_search[5] = {1, 2, 3, 4, 5};

    int * search_address = &var_to_search[3];

    LookupAddressVisitor visitor("var_to_search", &var_to_search, search_address);

    // ACT
    bool success = data_type->accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult().toString();
    std::string expected = "var_to_search[3]";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupAddressVisitorTest, composite1) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassOne");
    ClassOne var_to_search = {.a = 5, .b = 1.5};

    void * search_address = &(var_to_search.b);

    const DataType * search_type = dataTypeInator.resolve("double");

    LookupAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = data_type->accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult().toString();
    std::string expected = "var_to_search.b";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupAddressVisitorTest, composite_array) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassOne[4][3]");
    ClassOne var_to_search[4][3];

    void * search_address = &(var_to_search[2][2].b);

    const DataType * search_type = dataTypeInator.resolve("double");

    LookupAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = data_type->accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult().toString();
    std::string expected = "var_to_search[2][2].b";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupAddressVisitorTest, composite_array2) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search[2][2].c1.b);

    const DataType * search_type = dataTypeInator.resolve("double");

    LookupAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = data_type->accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult().toString();
    std::string expected = "var_to_search[2][2].c1.b";
    ASSERT_EQ(expected, actual);
}

// Explanation for the next two tests
// 

TEST_F(LookupAddressVisitorTest, search_type_ambiguous) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search[2][2].c1);

    const DataType * search_type = dataTypeInator.resolve("ClassOne");

    LookupAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = data_type->accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult().toString();
    std::string expected = "var_to_search[2][2].c1";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupAddressVisitorTest, search_type_ambiguous2) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search[2][2].c1);

    const DataType * search_type = dataTypeInator.resolve("int");

    LookupAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = data_type->accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult().toString();
    std::string expected = "var_to_search[2][2].c1.a";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupAddressVisitorTest, search_type_ambiguous3) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search);

    const DataType * search_type = dataTypeInator.resolve("int");

    LookupAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = data_type->accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult().toString();
    std::string expected = "var_to_search[0][0].x";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupAddressVisitorTest, search_type_ambiguous4) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search);

    const DataType * search_type = dataTypeInator.resolve("ClassTwo");

    LookupAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = data_type->accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult().toString();
    std::string expected = "var_to_search[0][0]";
    ASSERT_EQ(expected, actual);
}

TEST_F(LookupAddressVisitorTest, search_type_ambiguous5) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassTwo[4][3]");
    ClassTwo var_to_search[4][3];

    void * search_address = &(var_to_search);

    const DataType * search_type = dataTypeInator.resolve("ClassTwo[3]");

    LookupAddressVisitor visitor("var_to_search", &var_to_search, search_address, search_type);

    // ACT
    bool success = data_type->accept(&visitor);
    
    // ASSERT
    ASSERT_TRUE(success);
    std::string actual = visitor.getResult().toString();
    std::string expected = "var_to_search[0]";
    ASSERT_EQ(expected, actual);
}