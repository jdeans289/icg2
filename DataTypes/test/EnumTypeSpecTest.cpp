#include <gtest/gtest.h>
#include <stddef.h>
#include <iostream>
#include "TypeDictionary.hpp"
#include "Type/EnumDataType.hpp"
#include "Value/IntegerValue.hpp"

enum DayOfWeek {
    Sunday    = 1,
    Monday    = 2,
    Tuesday   = 3,
    Wednesday = 4,
    Thursday  = 5,
    Friday    = 6,
    Saturday  = 7
};

enum Fruit {
    Apple  = 0,
    Pear   = 1,
    Lemon  = 2,
    Orange = 3
};

// Framework
class EnumDataTypeTest : public ::testing::Test {
    protected:
    TypeDictionary *typeDictionary;
    EnumDictionary *enumDictionary;
    EnumDataTypeTest() {
        typeDictionary = new TypeDictionary;
        enumDictionary = new EnumDictionary;
    }
    ~EnumDataTypeTest() {
        delete typeDictionary;
        delete enumDictionary;
    }
    void SetUp() {}
    void TearDown() {}
};

bool addDayOfWeekEnumToTypeDictionary(TypeDictionary* typeDictionary, EnumDictionary* enumDictionary) {
    bool result = false;
    try {
        EnumDataType * dataType = new EnumDataType( enumDictionary, "DayOfWeek", sizeof(enum DayOfWeek) );
        dataType->addEnumerator( "Sunday",   1);
        dataType->addEnumerator( "Monday",   2);
        dataType->addEnumerator( "Tuesday",  3);
        dataType->addEnumerator( "Wednesday",4);
        dataType->addEnumerator( "Thursday", 5);
        dataType->addEnumerator( "Friday",   6);
        dataType->addEnumerator( "Saturday", 7);

        typeDictionary->addTypeDefinition( "DayOfWeek", dataType );
        result = dataType->validate();

    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
}

/* ================================================================================
                                         Test Cases
   ================================================================================
*/
TEST_F(EnumDataTypeTest, addEnumerator_normal ) {

    bool test_result = false;
    test_result = addDayOfWeekEnumToTypeDictionary(typeDictionary, enumDictionary);
    EXPECT_EQ(true, test_result);
}

TEST_F(EnumDataTypeTest, addEnumerator_exception ) {

    std::cout << "===== Expecting an error message about the re-definition of an enumerator type. =====" << std::endl;

    bool test_result = true;

    // Create a EnumDeclaration.
    EnumDataType * dataType = new EnumDataType( enumDictionary, "Fruit", sizeof(enum Fruit) );
    try {
        dataType->addEnumerator( "Apple",  0);
        dataType->addEnumerator( "Pear",   1);
        dataType->addEnumerator( "Lemon",  2);
        dataType->addEnumerator( "Orange", 3);
        dataType->addEnumerator( "Orange", 3);

        typeDictionary->addTypeDefinition( "Fruit", dataType );
        test_result = dataType->validate();

    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
        // We are expecting an exception because "Orange" is added twice.
        test_result = true;
    }
    EXPECT_EQ(true, test_result);
}

TEST_F(EnumDataTypeTest, getSize ) {

    EnumDataType * dataType = new EnumDataType( enumDictionary, "DayOfWeek", sizeof(enum DayOfWeek) );
    EXPECT_EQ( (int)sizeof(enum DayOfWeek), dataType->getSize());
}

TEST_F(EnumDataTypeTest, toString ) {

    EXPECT_EQ(true, addDayOfWeekEnumToTypeDictionary(typeDictionary, enumDictionary));

    const DataType* dataType = typeDictionary->lookup("DayOfWeek");
    ASSERT_TRUE(dataType != NULL) ;

    // Print the duplicate CompositeDataType.
    std::string s;
    s = dataType->toString();

    // Check the results.
    EXPECT_EQ("enum {\nSunday = 1,\n"
                      "Monday = 2,\n"
                      "Tuesday = 3,\n"
                      "Wednesday = 4,\n"
                      "Thursday = 5,\n"
                      "Friday = 6,\n"
                      "Saturday = 7}\n",s);
}

TEST_F(EnumDataTypeTest, AssignValue ) {

    EXPECT_EQ(true, addDayOfWeekEnumToTypeDictionary( typeDictionary, enumDictionary));

    const DataType* dataType = typeDictionary->lookup("DayOfWeek");
    ASSERT_TRUE(dataType != NULL) ;

    DayOfWeek dayOfWeek = Sunday;

    try {
        int value = enumDictionary->getValue( "Wednesday" );
        IntegerValue *integerValue = new IntegerValue( value );
        dataType->assignValue( &dayOfWeek, integerValue );
        delete integerValue;
    } catch ( const std::logic_error& e ) {
        std::cerr << e.what();
    }

    EXPECT_EQ(Wednesday, dayOfWeek);
}

TEST_F(EnumDataTypeTest, LookupEnumName ) {

    // ARRANGE
    EnumDataType * dataType = new EnumDataType( enumDictionary, "DayOfWeek", sizeof(enum DayOfWeek) );
    dataType->addEnumerator( "Sunday",   1);
    dataType->addEnumerator( "Monday",   2);
    dataType->addEnumerator( "Tuesday",  3);
    dataType->addEnumerator( "Wednesday",4);
    dataType->addEnumerator( "Thursday", 5);
    dataType->addEnumerator( "Friday",   6);
    dataType->addEnumerator( "Saturday", 7);

    typeDictionary->addTypeDefinition( "DayOfWeek", dataType );
    dataType->validate();

    // ACT
    std::string name = dataType->lookupEnumeratorName(4);

    // ASSERT
    std::string expected = "Wednesday";
    ASSERT_EQ(expected, name);
}

TEST_F(EnumDataTypeTest, LookupEnumNameNotFound ) {

    // ARRANGE
    EnumDataType * dataType = new EnumDataType( enumDictionary, "DayOfWeek", sizeof(enum DayOfWeek) );
    dataType->addEnumerator( "Sunday",   1);
    dataType->addEnumerator( "Monday",   2);
    dataType->addEnumerator( "Tuesday",  3);
    dataType->addEnumerator( "Wednesday",4);
    dataType->addEnumerator( "Thursday", 5);
    dataType->addEnumerator( "Friday",   6);
    dataType->addEnumerator( "Saturday", 7);

    typeDictionary->addTypeDefinition( "DayOfWeek", dataType );
    dataType->validate();

    // ACT
    std::string name = dataType->lookupEnumeratorName(1000000);

    // ASSERT
    std::string expected = "";
    ASSERT_EQ(expected, name);
}

