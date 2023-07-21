#include <gtest/gtest.h>
#include <stddef.h>
#include "TypeDictionary.hpp"
#include "DataTypeInator.hpp"
#include "Type/CompositeDataType.hpp"
#include "Type/SpecifiedPrimitiveDataType.hpp"
#include "Type/StringDataType.hpp"
#include "DataTypeTestSupport.hpp"

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
         compTypeSpec = new CompositeDataType( "ClassOne", sizeof(ClassOne), &construct<ClassOne>, &destruct<ClassOne>);
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
    test_passed  = typeDictionary->validate(&dataTypeInator);
    EXPECT_EQ( true, test_passed);

    // Verify that the correct DataType can be retrieved from the TypeDictionary by name.
    const DataType* dataType = typeDictionary->lookup("ClassOne");
    EXPECT_EQ( (void*)compTypeSpec, (void*)dataType);
}

TEST_F(TypeDictionaryTest, addTypeDefinition_2) {

    /* An attempt to redefine a type should throw an exception. */

    int test_result = 0;

    try {
        typeDictionary->addTypeDefinition("char", new SpecifiedPrimitiveDataType<char>() );
    } catch (const std::logic_error& e) {
        std::cerr << "NOTE: Exception is expected as part of the test." << std::endl;
        std::cerr << e.what() << std::endl;
        test_result = 1;
    }

    EXPECT_EQ( 1, test_result);
}


TEST_F(TypeDictionaryTest, namespaces_lookup) {
    // ARRANGE
    // std::string is in the builtins

    // ACT
    const DataType * type = typeDictionary->lookup("std::string");

    // ASSERT
    ASSERT_TRUE(type != NULL);
    ASSERT_EQ("std::string", type->toString());
}


TEST_F(TypeDictionaryTest, namespaces_definition) {
    // ARRANGE
    typeDictionary->addTypeDefinition("A::B::C::D", new SpecifiedPrimitiveDataType<double>());

    // ACT
    const DataType * type = typeDictionary->lookup("A::B::C::D");

    // ASSERT
    ASSERT_TRUE(type != NULL);
    ASSERT_EQ("double", type->toString());
}

TEST_F(TypeDictionaryTest, namespaces_failed_lookup) {
    // ARRANGE
    typeDictionary->addTypeDefinition("A::B::C::D", new SpecifiedPrimitiveDataType<double>());

    // ACT
    const DataType * type = typeDictionary->lookup("A::B::no_such_namespace::no_such_type");

    // ASSERT
    ASSERT_TRUE(type == NULL);
}

TEST_F(TypeDictionaryTest, namespaces_failed_lookup_2) {
    // ARRANGE
    typeDictionary->addTypeDefinition("A::B::C::D", new SpecifiedPrimitiveDataType<double>());

    // ACT
    const DataType * type = typeDictionary->lookup("A::B::C::no_such_type");

    // ASSERT
    ASSERT_TRUE(type == NULL);
}
