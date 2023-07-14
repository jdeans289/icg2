
#include <gtest/gtest.h>
#include <stddef.h>

#include "DataTypeInator.hpp"

#include "Type/CompositeDataType.hpp"
#include "Type/PrimitiveDataType.hpp"

#include "Value/CompositeValue.hpp"
#include "Value/ArrayValue.hpp"

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
    ASSERT_EQ( type_b.getMemberCount(), type_a.getMemberCount());
}

TEST_F(CompositeDataTypeTest, assignValue_1) {

    /* Requirement: */

    ClassOne classOne;
    classOne.a = 0;
    classOne.b = 0.0;

    EXPECT_EQ(true, addClassOneToTypeDictionary( dataTypeInator ));

    const DataType* dataType = dataTypeInator->resolve("ClassOne");
    ASSERT_TRUE(dataType != NULL) ;

    CompositeValue *
        classOneValue = new  CompositeValue();
        classOneValue->addMember( new IntegerValue(172));
        classOneValue->addMember( new FloatingPointValue(3.1415927));

    dataType->assignValue(&classOne, classOneValue);

    EXPECT_EQ( 172, classOne.a);
    EXPECT_NEAR( 3.1415927, classOne.b, 0.0001);
}

TEST_F(CompositeDataTypeTest, assignValue_2) {

    /* Requirment:  */
    ClassTwo classTwo;
    classTwo.x = 0;
    classTwo.y = 0.0;
    classTwo.c1.a = 0;
    classTwo.c1.b = 0.0;

    EXPECT_EQ(true, addClassOneToTypeDictionary( dataTypeInator ));
    EXPECT_EQ(true, addClassTwoToTypeDictionary( dataTypeInator ));

    const DataType* dataType = dataTypeInator->resolve("ClassTwo");
    ASSERT_TRUE(dataType != NULL) ;

    CompositeValue * classOneValue = new  CompositeValue();
        classOneValue->addMember( new IntegerValue(172));
        classOneValue->addMember( new FloatingPointValue(3.1415927));

    CompositeValue * classTwoValue = new  CompositeValue();
        classTwoValue->addMember( new IntegerValue(278));
        classTwoValue->addMember( new FloatingPointValue(2.7182818));
        classTwoValue->addMember( classOneValue );

    dataType->assignValue(&classTwo, classTwoValue);

    delete classTwoValue;

    EXPECT_EQ( 172, classTwo.c1.a);
    EXPECT_NEAR( 3.1415927, classTwo.c1.b, 0.0001);
    EXPECT_EQ( 278, classTwo.x);
    EXPECT_NEAR( 2.7182818, classTwo.y, 0.0001);
}

TEST_F(CompositeDataTypeTest, assignValue_3) {

    /* Requirment:  */
    ClassThree classThree;
    classThree.pos[0] = 0.0;
    classThree.pos[1] = 0.0;
    classThree.vel[0] = 0.0;
    classThree.vel[1] = 0.0;

    EXPECT_EQ(true, addClassThreeToTypeDictionary( dataTypeInator ));

    const DataType* dataType = dataTypeInator->resolve("ClassThree");
    ASSERT_TRUE(dataType != NULL) ;

    ArrayValue * posVal = new ArrayValue();
        posVal->pushElem(new FloatingPointValue(2.3));
        posVal->pushElem(new FloatingPointValue(5.7));
    ArrayValue * velVal = new ArrayValue();
        velVal->pushElem(new FloatingPointValue(4.3));
        velVal->pushElem(new FloatingPointValue(7.7));
    CompositeValue * classThreeValue = new  CompositeValue();
        classThreeValue->addMember( posVal );
        classThreeValue->addMember( velVal );

    dataType->assignValue(&classThree, classThreeValue);

    EXPECT_NEAR( 2.3, classThree.pos[0], 0.0001);
    EXPECT_NEAR( 5.7, classThree.pos[1], 0.0001);
    EXPECT_NEAR( 4.3, classThree.vel[0], 0.0001);
    EXPECT_NEAR( 7.7, classThree.vel[1], 0.0001);

}

TEST_F(CompositeDataTypeTest, assignValue_4) {

    /* Requirement:  */
    ClassFour classFour;
    classFour.x[0] = 0.0;
    classFour.x[1] = 0.0;
    classFour.f1 = 0;
    classFour.f2 = 0;
    classFour.f3 = 0;

    EXPECT_EQ(true, addClassFourToTypeDictionary( dataTypeInator ));

    const DataType* dataType = dataTypeInator->resolve("ClassFour");
    ASSERT_TRUE(dataType != NULL) ;

    ArrayValue * xValue = new  ArrayValue();
        xValue->pushElem( new FloatingPointValue(2.3));
        xValue->pushElem( new FloatingPointValue(5.7));
    IntegerValue * f1Value = new IntegerValue(2);
    IntegerValue * f2Value = new IntegerValue(13);
    IntegerValue * f3Value = new IntegerValue(55);

    CompositeValue * classFourValue = new  CompositeValue();
        classFourValue->addMember( xValue );
        classFourValue->addMember( f1Value );
        classFourValue->addMember( f2Value );
        classFourValue->addMember( f3Value );

    dataType->assignValue(&classFour, classFourValue);

    EXPECT_NEAR( 2.3, classFour.x[0], 0.0001);
    EXPECT_NEAR( 5.7, classFour.x[1], 0.0001);
    EXPECT_EQ(  2, classFour.f1);
    EXPECT_EQ( 13, classFour.f2);
    EXPECT_EQ( 55, classFour.f3);

}

TEST_F(CompositeDataTypeTest, assignValue_5) {

    EXPECT_EQ(true, addClassFiveToTypeDictionary( dataTypeInator ));

    const DataType* dataType = dataTypeInator->resolve("ClassFive");
    ASSERT_TRUE(dataType != NULL) ;

    ClassFive* classFive = (ClassFive*)dataType->createInstance(1);

    CompositeValue * classFiveValue = new  CompositeValue();
        classFiveValue->addMember( new FloatingPointValue(7.3));
        classFiveValue->addMember( new IntegerValue(13));

    dataType->assignValue(classFive, classFiveValue);

    EXPECT_NEAR( 7.3, classFive->x, 0.0001);
    EXPECT_EQ( 13, classFive->count);
}

