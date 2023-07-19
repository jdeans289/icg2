
#include <gtest/gtest.h>
#include <stddef.h>

#include "DataTypeInator.hpp"

#include "Type/CompositeDataType.hpp"
#include "Type/PrimitiveDataType.hpp"

#include "DataTypeTestSupport.hpp"

// Framework
class CompositeDataTypeTest : public ::testing::Test {
    protected:
    DataTypeInator *dataTypeInator;
    CompositeDataTypeTest() { dataTypeInator = new DataTypeInator; }
    ~CompositeDataTypeTest() { delete dataTypeInator; }
    void SetUp() {}
    void TearDown() {}
};

TEST_F(CompositeDataTypeTest, validate ) {

    /* Requirement: CompositeDataType::validate() method shall attempt to resolve
       all of it's prerequisite type names to actual DataTypes. If it succeeds,
       it shall return true, otherwise it shall return false. */

    bool test_result;

    // Create a CompositeDataType.
    /* Note that this DataType is dependent on the types "int" and "double". Both
       both of these named type dependencies must be resolvable, via the DataTypeInator,
       to their respective DataTypes. */

    test_result = addClassOneToTypeDictionary( dataTypeInator );
    EXPECT_EQ(true, test_result);

    test_result = addClassTwoToTypeDictionary( dataTypeInator );
    EXPECT_EQ(true, test_result);

    test_result = addClassThreeToTypeDictionary( dataTypeInator );
    EXPECT_EQ(true, test_result);

    test_result = addClassFourToTypeDictionary( dataTypeInator );
    EXPECT_EQ(true, test_result);

    test_result = addClassFiveToTypeDictionary( dataTypeInator );
    EXPECT_EQ(true, test_result);

}

TEST_F(CompositeDataTypeTest, toString_1 ) {

    EXPECT_EQ(true, addClassOneToTypeDictionary( dataTypeInator ));

    const DataType* dataType = dataTypeInator->resolve("ClassOne");

    ASSERT_TRUE(dataType != NULL) ;

    // Print the duplicate CompositeDataType.
    std::string s;
    s = dataType->toString();

    // Check the results.
    EXPECT_EQ("composite {\nint a;\ndouble b;\n}\n", s);
}

TEST_F(CompositeDataTypeTest, toString_2 ) {

    EXPECT_EQ(true, addClassOneToTypeDictionary( dataTypeInator ));
    EXPECT_EQ(true, addClassTwoToTypeDictionary( dataTypeInator ));

    const DataType* dataType = dataTypeInator->resolve("ClassTwo");
    ASSERT_TRUE(dataType != NULL) ;

    // Print the duplicate CompositeDataType.
    std::stringstream ss;
    ss << dataType->toString();

    // Check the results.
    EXPECT_EQ("composite {\nint x;\ndouble y;\nClassOne c1;\n}\n", ss.str());
}

TEST_F(CompositeDataTypeTest, toString_3 ) {

    EXPECT_EQ(true, addClassThreeToTypeDictionary( dataTypeInator ));

    const DataType* dataType = dataTypeInator->resolve("ClassThree");
    ASSERT_TRUE(dataType != NULL) ;

    // Print the duplicate CompositeDataType.
    std::stringstream ss;
    ss << dataType->toString();

    // Check the results.
    EXPECT_EQ("composite {\ndouble pos[2];\ndouble vel[2];\n}\n", ss.str());
}

TEST_F(CompositeDataTypeTest, copy_constructor ) {

    EXPECT_EQ(true, addClassOneToTypeDictionary( dataTypeInator ));

    const DataType* dataType = dataTypeInator->resolve("ClassOne");
    ASSERT_TRUE(dataType != NULL) ;

    const DataType * copy = new CompositeDataType(*(const CompositeDataType*)dataType);

    // Print the duplicate CompositeDataType.
    std::stringstream ss;
    ss << copy->toString();

    // Check the results.
    EXPECT_EQ("composite {\nint a;\ndouble b;\n}\n", ss.str());

    delete copy;
}

TEST_F(CompositeDataTypeTest, operator_equal ) {

    EXPECT_EQ(true, addClassOneToTypeDictionary( dataTypeInator ));

    const DataType* dataType = dataTypeInator->resolve("ClassOne");
    ASSERT_TRUE(dataType != NULL) ;

    CompositeDataType copy = *(const CompositeDataType*)dataType;

    // Print the duplicate CompositeDataType.
    std::stringstream ss;
    ss << copy.toString();

    // Check the results.
    EXPECT_EQ("composite {\nint a;\ndouble b;\n}\n", ss.str());
}


TEST_F(CompositeDataTypeTest, getSize ) {

    /* Requirement: CompositeDataType::getSize() shall return the size passed in
       via the constructor. */

    CompositeDataType * compTypeSpec = new CompositeDataType( dataTypeInator,
                                                              "ClassOne",
                                                              sizeof(ClassOne),
                                                              &construct<ClassOne>,
                                                              &destruct<ClassOne>);

    compTypeSpec->addRegularMember( "a", offsetof(ClassOne, a), "int");
    compTypeSpec->addRegularMember( "b", offsetof(ClassOne, b), "double");

    EXPECT_EQ( sizeof(ClassOne), compTypeSpec->getSize());

}

TEST_F(CompositeDataTypeTest, assignment_operator ) {
    // ARRANGE
    CompositeDataType  type_a(dataTypeInator,
                                    "ClassOne",
                                    sizeof(ClassOne),
                                    &construct<ClassOne>,
                                    &destruct<ClassOne>);

    type_a.addRegularMember( "a", offsetof(ClassOne, a), "int");
    type_a.addRegularMember( "b", offsetof(ClassOne, b), "double");

    CompositeDataType  type_b(dataTypeInator,
                                    "SomeOtherClass",
                                    4,
                                    NULL,
                                    NULL);

    type_b.addRegularMember( "somethingidk", 0, "int");

    type_a.validate();
    type_b.validate();


    // ACT
    type_a = type_b;

    // ASSERT
    EXPECT_EQ( type_b.getSize(), type_a.getSize());
    EXPECT_EQ( std::string("SomeOtherClass"), type_a.getTypeSpecName());
    ASSERT_EQ( type_b.getNormalMemberCount(), type_a.getNormalMemberCount());
}

