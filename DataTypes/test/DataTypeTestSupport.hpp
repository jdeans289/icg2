#include <stddef.h>
#include <stdlib.h>
#include "DataTypeInator.hpp"
#include "Type/EnumDictionary.hpp"
#include "Type/SpecifiedCompositeType.hpp"

// ClassOne contains regular, primitive, unarrayed, data-members.
class ClassOne {
    public:
    int a;
    double b;
};

// ClassTwo contains regular, primitive and composite data-members.
class ClassTwo {
    public:
    int x;
    double y;
    ClassOne c1;
};

// ClassThree contains regular, primitive, arrayed data-members.
class ClassThree {
    public:
    double pos[2];
    double vel[2];
};

// ClassFour contains bitfield data-members as well as regular, primitive, arrayed data-members.
class ClassFour {
    public:
    double x[2];
    short f1 : 3;
    short f2 : 5;
    short f3 : 7;
};

// ClassFive contains static and regular, primitive, arrayed data-members.
class ClassFive {
    public:
    double x;
    static int count;
};

// ClassSix contains a character ptr and and std::string
class ClassSix {
    public:
    const char * char_ptr;
    std::string str;
};

class ClassWithNoPointers {
    public:
    int a;
    std::string b;
    double c[8][7][6][5];
};

class ClassWithPointer {
    public:
    void * a;
};

class ClassWithNestedClasses {
    public:
    ClassWithNoPointers a;
    ClassWithPointer b;
};


class VecClass {
    public:
    std::vector<int> v;
};


bool addClassOneToTypeDictionary(DataTypeInator* dataTypeInator);
bool addClassTwoToTypeDictionary(DataTypeInator* dataTypeInator);
bool addClassThreeToTypeDictionary(DataTypeInator* dataTypeInator);
// bool addClassFourToTypeDictionary(DataTypeInator* dataTypeInator);
// bool addClassFiveToTypeDictionary(DataTypeInator* dataTypeInator);
// bool addClassFiveToTypeDictionary(DataTypeInator* dataTypeInator);
bool addClassSixToTypeDictionary(DataTypeInator* dataTypeInator);
bool addPointerTestClassesToDictionary(DataTypeInator* dataTypeInator);
bool addVecClassToDictionary(DataTypeInator* dataTypeInator);


enum DayOfWeek {
    Sunday    = 1,
    Monday    = 2,
    Tuesday   = 3,
    Wednesday = 4,
    Thursday  = 5,
    Friday    = 6,
    Saturday  = 7
};

bool addDayOfWeekEnumToTypeDictionary(DataTypeInator* dataTypeInator, EnumDictionary* enumDictionary);

// Template specializations for the types

template <>
class SpecifiedCompositeType<VecClass> : public CompositeDataType {
    public:
    SpecifiedCompositeType(std::string name) : CompositeDataType(name, sizeof(VecClass), &construct_composite<VecClass>, &destruct_composite<VecClass>) {}

    MemberMap& getMemberMap () override {
        using type_to_add = VecClass;

        static MemberMap member_map = {
            {"v", StructMember("v", "std::vector<int>", offsetof(type_to_add, v))},
        };
        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<VecClass>*> (this))->getMemberMap();
    }
};

template <>
class SpecifiedCompositeType<ClassOne> : public CompositeDataType {

    public:
    SpecifiedCompositeType() : CompositeDataType("ClassOne", sizeof(ClassOne), &construct_composite<ClassOne>, &destruct_composite<ClassOne>) {}

    template<typename U>
    static MemberMap applyMembersToDerived () {
        MemberMap derived_member_map = {
            {"a", StructMember("a", "int", offsetof(U, ClassOne::a))},
            {"b", StructMember("b", "double", offsetof(U, ClassOne::b))},
        };

        return derived_member_map;
    }    

    MemberMap& getMemberMap () override {
        static bool initialized = false;
        static MemberMap member_map = {
            {"a", StructMember("a", "int", offsetof(ClassOne, a))},
            {"b", StructMember("b", "double", offsetof(ClassOne, b))},
        };

        if (!initialized) {
            initialized = true;
        }

        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<ClassOne>*> (this))->getMemberMap();
    }
};

template <>
class SpecifiedCompositeType<ClassTwo> : public CompositeDataType {

    public:
    SpecifiedCompositeType() : CompositeDataType("ClassTwo", sizeof(ClassTwo), &construct_composite<ClassTwo>, &destruct_composite<ClassTwo>) {}

    template<typename U>
    static MemberMap applyMembersToDerived () {
        MemberMap derived_member_map = {
            {"x", StructMember("x", "int", offsetof(U, ClassTwo::x))},
            {"y", StructMember("y", "double", offsetof(U, ClassTwo::y))},
            {"c1", StructMember("c1", "ClassOne", offsetof(U, ClassTwo::c1))},
        };

        return derived_member_map;
    }    

    MemberMap& getMemberMap () override {
        static bool initialized = false;
        static MemberMap member_map = {
            {"x", StructMember("x", "int", offsetof(ClassTwo, x))},
            {"y", StructMember("y", "double", offsetof(ClassTwo, y))},
            {"c1", StructMember("c1", "ClassOne", offsetof(ClassTwo, c1))},
        };

        if (!initialized) {
            initialized = true;
        }

        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<ClassTwo>*> (this))->getMemberMap();
    }
};

template <>
class SpecifiedCompositeType<ClassThree> : public CompositeDataType {

    public:
    SpecifiedCompositeType() : CompositeDataType("ClassThree", sizeof(ClassThree), &construct_composite<ClassThree>, &destruct_composite<ClassThree>) {}

    template<typename U>
    static MemberMap applyMembersToDerived () {
        MemberMap derived_member_map = {
            {"pos", StructMember("pos", "double[2]", offsetof(U, ClassThree::pos))},
            {"vel", StructMember("vel", "double[2]", offsetof(U, ClassThree::vel))},
        };

        return derived_member_map;
    }    

    MemberMap& getMemberMap () override {
        static bool initialized = false;
        static MemberMap member_map = {
            {"pos", StructMember("pos", "double[2]", offsetof(ClassThree, pos))},
            {"vel", StructMember("vel", "double[2]", offsetof(ClassThree, vel))},
        };

        if (!initialized) {
            // Base class member instantiations go here
            initialized = true;
        }

        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<ClassThree>*> (this))->getMemberMap();
    }
};

template <>
class SpecifiedCompositeType<ClassSix> : public CompositeDataType {
    public:
    SpecifiedCompositeType() : CompositeDataType("ClassSix", sizeof(ClassSix), &construct_composite<ClassSix>, &destruct_composite<ClassSix>) {}

    MemberMap& getMemberMap () override {
        using type_to_add = ClassSix;

        static MemberMap member_map = {
            {"char_ptr", StructMember("char_ptr", "char *", offsetof(type_to_add, char_ptr))},
            {"str", StructMember("str", "std::string", offsetof(type_to_add, str))},
        };
        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<ClassSix>*> (this))->getMemberMap();
    }
};

template <>
class SpecifiedCompositeType<ClassWithNoPointers> : public CompositeDataType {
    public:
    SpecifiedCompositeType() : CompositeDataType("ClassWithNoPointers", sizeof(ClassWithNoPointers), &construct_composite<ClassWithNoPointers>, &destruct_composite<ClassWithNoPointers>) {}

    MemberMap& getMemberMap () override {
        using type_to_add = ClassWithNoPointers;

        static MemberMap member_map = {
            {"a", StructMember("a", "int", offsetof(type_to_add, a))},
            {"b", StructMember("b", "std::string", offsetof(type_to_add, b))},
            {"c", StructMember("c", "double[8][7][6][5]", offsetof(type_to_add, c))},
        };
        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<ClassWithNoPointers>*> (this))->getMemberMap();
    }
};

template <>
class SpecifiedCompositeType<ClassWithPointer> : public CompositeDataType {
    public:
    SpecifiedCompositeType() : CompositeDataType("ClassWithPointer", sizeof(ClassWithPointer), &construct_composite<ClassWithPointer>, &destruct_composite<ClassWithPointer>) {}

    MemberMap& getMemberMap () override {
        using type_to_add = ClassWithPointer;

        static MemberMap member_map = {
            {"a", StructMember("a", "void*", offsetof(type_to_add, a))},
        };
        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<ClassWithPointer>*> (this))->getMemberMap();
    }
};

template <>
class SpecifiedCompositeType<ClassWithNestedClasses> : public CompositeDataType {
    public:
    SpecifiedCompositeType() : CompositeDataType("ClassWithNestedClasses", sizeof(ClassWithNestedClasses), &construct_composite<ClassWithNestedClasses>, &destruct_composite<ClassWithNestedClasses>) {}

    MemberMap& getMemberMap () override {
        using type_to_add = ClassWithNestedClasses;

        static MemberMap member_map = {
            {"a", StructMember("a", "ClassWithNoPointers", offsetof(type_to_add, a))},
            {"b", StructMember("b", "ClassWithPointer", offsetof(type_to_add, b))},

        };
        return member_map;
    }

    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<ClassWithNestedClasses>*> (this))->getMemberMap();
    }
};