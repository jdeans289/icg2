#include <gtest/gtest.h>
#include <stddef.h>
#include "TypeDictionary.hh"
#include "DataTypeInator.hh"
#include "Type/CompositeDataType.hh"
#include "Value/CompositeValue.hh"
#include "Type/PrimitiveDataType.hh"
#include "DataTypeTestSupport.hh"

// Framework
class TypeDictionaryTest : public ::testing::Test {
    protected:
    TypeDictionary *typeDictionary;
    TypeDictionaryTest() { 
        typeDictionary = new TypeDictionary;
        typeDictionary->addBuiltinTypes();
    }
    ~TypeDictionaryTest() { delete typeDictionary; }
    void SetUp() {}
    void TearDown() {}
};


TEST_F(TypeDictionaryTest, lookup_1) {

    /* TypeDictionary::lookup should return NULL if the specified
       typeName is not defined in the TypeDictionary.*/

    const DataType* dataType = typeDictionary->lookup("non-existent-type");

    EXPECT_EQ( NULL, dataType);
}

TEST_F(TypeDictionaryTest, lookup_2) {

    /* TypeDictionary should be pre-loaded with all of the builtin types */

    const DataType* dataType;

    dataType = typeDictionary->lookup("void");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("char");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("short");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("int");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("long");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("wchar_t");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("long long");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("unsigned char");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("unsigned short");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("unsigned int");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("unsigned long");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("unsigned long long");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("float");
    EXPECT_NE( (void*)NULL, dataType);

    dataType = typeDictionary->lookup("double");
    EXPECT_NE( (void*)NULL, dataType);

}

TEST_F(TypeDictionaryTest, addTypeDefinition_1) {

    /* Should be able to add a <std:string/DataType> pair (a type definition) to the TypeDictionary. */
    bool test_passed = true;

    CompositeDataType * compTypeSpec;

    DataTypeInator dataTypeInator(typeDictionary);

    try {
         compTypeSpec = new CompositeDataType(&dataTypeInator, "ClassOne", sizeof(ClassOne), &construct<ClassOne>, &destruct<ClassOne>);
         compTypeSpec->addRegularMember( "a", offsetof(ClassOne, a), "int");
         compTypeSpec->addRegularMember( "b", offsetof(ClassOne, b), "double");

        typeDictionary->addTypeDefinition("ClassOne", compTypeSpec);

    } catch (const std::logic_error& e) {
        std::cerr << e.what();
        test_passed = false;
    }

    // Verify that addTypeDefinition didn't throw an exception.
    EXPECT_EQ( true, test_passed);

    // Verify that the types can be validated.
    test_passed  = typeDictionary->validate();
    EXPECT_EQ( true, test_passed);

    // Verify that the correct DataType can be retrieved from the TypeDictionary by name.
    const DataType* dataType = typeDictionary->lookup("ClassOne");
    EXPECT_EQ( (void*)compTypeSpec, (void*)dataType);
}

TEST_F(TypeDictionaryTest, addTypeDefinition_2) {

    /* An attempt to redefine a type should throw an exception. */

    int test_result = 0;

    try {
        typeDictionary->addTypeDefinition("char", new PrimitiveDataType<char>() );
    } catch (const std::logic_error& e) {
        std::cerr << "NOTE: Exception is expected as part of the test." << std::endl;
        std::cerr << e.what() << std::endl;
        test_result = 1;
    }

    EXPECT_EQ( 1, test_result);
}

