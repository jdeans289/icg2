#include "DataTypeTestSupport.hpp"

#include "Type/SpecifiedCompositeType.hpp"
#include "Type/SpecifiedSequenceDataType.hpp"
#include "Type/PrimitiveDataType.hpp"
#include "Type/EnumDataType.hpp"

int ClassFive::count = 0;

static short get_ClassFour__f1(void* addr) { return  ((ClassFour*)addr)->f1; }
static void set_ClassFour__f1(void* addr, short v) { ((ClassFour*)addr)->f1 = v; }

static short get_ClassFour__f2(void* addr) { return  ((ClassFour*)addr)->f2; }
static void set_ClassFour__f2(void* addr, short v) { ((ClassFour*)addr)->f2 = v; }

static short get_ClassFour__f3(void* addr) { return  ((ClassFour*)addr)->f3; }
static void set_ClassFour__f3(void* addr, short v) { ((ClassFour*)addr)->f3 = v; }




bool addClassOneToTypeDictionary(DataTypeInator* dataTypeInator) {

    dataTypeInator->addToDictionary("ClassOne", new SpecifiedCompositeType<ClassOne>);
    return dataTypeInator->validateDictionary();
}

bool addClassTwoToTypeDictionary(DataTypeInator* dataTypeInator) {

    dataTypeInator->addToDictionary("ClassTwo", new SpecifiedCompositeType<ClassTwo>);
    return dataTypeInator->validateDictionary();
}

bool addClassThreeToTypeDictionary(DataTypeInator* dataTypeInator) {

    dataTypeInator->addToDictionary("ClassThree", new SpecifiedCompositeType<ClassThree>);
    return dataTypeInator->validateDictionary();
}

bool addClassSixToTypeDictionary(DataTypeInator* dataTypeInator) {

    dataTypeInator->addToDictionary("ClassSix", new SpecifiedCompositeType<ClassSix>);
    return dataTypeInator->validateDictionary();
}

bool addVecClassToDictionary(DataTypeInator* dataTypeInator) {
    dataTypeInator->addToDictionary("std::vector<int>", new SpecifiedSequenceDataType<std::vector<int>>("std::vector<int>"));
    dataTypeInator->addToDictionary("VecClass", new SpecifiedCompositeType<VecClass>("VecClass"));
    return dataTypeInator->validateDictionary();
}

bool addPointerTestClassesToDictionary(DataTypeInator* dataTypeInator) {
    dataTypeInator->addToDictionary("ClassWithNoPointers", new SpecifiedCompositeType<ClassWithNoPointers>);
    dataTypeInator->addToDictionary("ClassWithPointer", new SpecifiedCompositeType<ClassWithPointer>);
    dataTypeInator->addToDictionary("ClassWithNestedClasses", new SpecifiedCompositeType<ClassWithNestedClasses>);

    return dataTypeInator->validateDictionary();
}

bool addDayOfWeekEnumToTypeDictionary(DataTypeInator* dataTypeInator,  EnumDictionary* enumDictionary) {

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

            dataTypeInator->addToDictionary( "DayOfWeek", dataType );
            result = dataType->validate(dataTypeInator);
    } catch( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
}
