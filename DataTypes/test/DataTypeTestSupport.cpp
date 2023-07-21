#include "DataTypeTestSupport.hpp"

#include "Type/CompositeDataType.hpp"
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

    bool result = false;
    try {
        CompositeDataType* classOneTypeSpec =
            new CompositeDataType( "ClassOne", sizeof(ClassOne), &construct<ClassOne>, &destruct<ClassOne> );
            classOneTypeSpec->addRegularMember( "a", offsetof(ClassOne, a), "int");
            classOneTypeSpec->addRegularMember( "b", offsetof(ClassOne, b), "double");

        dataTypeInator->addToDictionary("ClassOne", classOneTypeSpec);
        result = classOneTypeSpec->validate(dataTypeInator);
    } catch( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
}

bool addClassTwoToTypeDictionary(DataTypeInator* dataTypeInator) {

    bool result = false;
    try {
        CompositeDataType * classTwoTypeSpec =
            new CompositeDataType( "ClassTwo", sizeof(ClassTwo), &construct<ClassTwo>, &destruct<ClassTwo>);
            classTwoTypeSpec->addRegularMember( "x", offsetof(ClassTwo, x), "int");
            classTwoTypeSpec->addRegularMember( "y", offsetof(ClassTwo, y), "double");
            classTwoTypeSpec->addRegularMember( "c1", offsetof(ClassTwo, c1), "ClassOne");

        dataTypeInator->addToDictionary("ClassTwo", classTwoTypeSpec);

        result = classTwoTypeSpec->validate(dataTypeInator);
    } catch( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
}

bool addClassThreeToTypeDictionary(DataTypeInator* dataTypeInator) {

    bool result = false;
    try {
        CompositeDataType * classThreeTypeSpec =
            new CompositeDataType( "ClassThree", sizeof(ClassThree), &construct<ClassThree>, &destruct<ClassThree>);
            // int x_dims[] = {2};
            classThreeTypeSpec->addRegularMember( "pos", offsetof(ClassThree, pos), "double[2]");
            // int y_dims[] = {2};
            classThreeTypeSpec->addRegularMember( "vel", offsetof(ClassThree, vel), "double[2]");

        dataTypeInator->addToDictionary("ClassThree", classThreeTypeSpec);

        result = classThreeTypeSpec->validate(dataTypeInator);
    } catch( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
}

bool addClassFourToTypeDictionary(DataTypeInator* dataTypeInator) {

    bool result = false;
    try {
        CompositeDataType * classFourTypeSpec =
            new CompositeDataType( "ClassFour", sizeof(ClassFour), &construct<ClassFour>, &destruct<ClassFour>);
            classFourTypeSpec->addRegularMember( "x", offsetof(ClassFour, x), "double[2]");
            classFourTypeSpec->addBitFieldMember( "f1", get_ClassFour__f1, set_ClassFour__f1);
            classFourTypeSpec->addBitFieldMember( "f2", get_ClassFour__f2, set_ClassFour__f2);
            classFourTypeSpec->addBitFieldMember( "f3", get_ClassFour__f3, set_ClassFour__f3);

        dataTypeInator->addToDictionary("ClassFour", classFourTypeSpec);

        result = classFourTypeSpec->validate(dataTypeInator);
    } catch( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
}

bool addClassFiveToTypeDictionary(DataTypeInator* dataTypeInator) {

    bool result = false;
    try {
        CompositeDataType * classFiveTypeSpec =
            new CompositeDataType( "ClassFive", sizeof(ClassFive), &construct<ClassFive>, &destruct<ClassFive>);
            classFiveTypeSpec->addRegularMember( "x", offsetof(ClassFive, x), "double");
            classFiveTypeSpec->addStaticMember( "count", &ClassFive::count, "int" );

        dataTypeInator->addToDictionary("ClassFive", classFiveTypeSpec);

        result = classFiveTypeSpec->validate(dataTypeInator);
    } catch( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
}

bool addClassSixToTypeDictionary(DataTypeInator* dataTypeInator) {

    bool result = false;
    try {
        CompositeDataType * classSixTypeSpec =
            new CompositeDataType( "ClassSix", sizeof(ClassSix), &construct<ClassSix>, &destruct<ClassSix>);
            classSixTypeSpec->addRegularMember( "char_ptr", offsetof(ClassSix, char_ptr), "char *");
            classSixTypeSpec->addRegularMember( "str", offsetof(ClassSix, str), "std::string");

        dataTypeInator->addToDictionary("ClassSix", classSixTypeSpec);

        result = classSixTypeSpec->validate(dataTypeInator);
    } catch( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
}


bool addPointerTestClassesToDictionary(DataTypeInator* dataTypeInator) {
    bool result = false;

    try {
        CompositeDataType * ClassWithNoPointersType = new CompositeDataType ( "ClassWithNoPointers", sizeof(ClassWithNoPointers),    
                                                                            &construct<ClassWithNoPointers>, &destruct<ClassWithNoPointers>);

        ClassWithNoPointersType->addRegularMember( "a", offsetof(ClassWithNoPointers, a), "int");
        ClassWithNoPointersType->addRegularMember( "b", offsetof(ClassWithNoPointers, b), "std::string");
        ClassWithNoPointersType->addRegularMember( "c", offsetof(ClassWithNoPointers, c), "double[8][7][6][5]");

        dataTypeInator->addToDictionary("ClassWithNoPointers", ClassWithNoPointersType);

        CompositeDataType * ClassWithPointerType = new CompositeDataType ( "ClassWithPointer", sizeof(ClassWithPointer),    
                                                                    &construct<ClassWithPointer>, &destruct<ClassWithPointer>);
        
        ClassWithPointerType->addRegularMember( "a", offsetof(ClassWithPointer, a), "void *");
        dataTypeInator->addToDictionary("ClassWithPointer", ClassWithPointerType);

        CompositeDataType * ClassWithNestedClassesType = new CompositeDataType ( "ClassWithNestedClasses", sizeof(ClassWithNestedClasses),    
                                                                    &construct<ClassWithNestedClasses>, &destruct<ClassWithNestedClasses>);
        
        ClassWithNestedClassesType->addRegularMember( "a", offsetof(ClassWithNestedClasses, a), "ClassWithNoPointers");
        ClassWithNestedClassesType->addRegularMember( "b", offsetof(ClassWithNestedClasses, b), "ClassWithPointer");

        dataTypeInator->addToDictionary("ClassWithNestedClasses", ClassWithNestedClassesType);

        result = ClassWithNoPointersType->validate(dataTypeInator);
        result &= ClassWithPointerType->validate(dataTypeInator);
        result &= ClassWithNestedClassesType->validate(dataTypeInator);

    } catch( const std::logic_error& e ) {
        std::cerr << e.what();
        result = false;
    }
    return result;
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
