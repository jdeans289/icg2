#include <stddef.h>
#include <stdlib.h>
#include "DataTypeInator.hh"
#include "Type/EnumDictionary.hh"

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

class ClassSix {
    public:
    const char * char_ptr;
    std::string str;
};

// An Allocator
template <typename T>
void* construct (int num) {
    T* temp = (T*)calloc( num, sizeof(T));
    for (int ii=0 ; ii<num ; ii++) { new( &temp[ii]) T(); }
    return ((void *)temp);
}

// A deAllocator
// Object constructed with placement new must call the destructor explicitly (https://isocpp.org/wiki/faq/dtors#placement-new)
template <typename T>
void destruct (void* address) {
    // free(address);
    T* temp = static_cast<T*> (address);
    temp->~T();
}

bool addClassOneToTypeDictionary(DataTypeInator* dataTypeInator);
bool addClassTwoToTypeDictionary(DataTypeInator* dataTypeInator);
bool addClassThreeToTypeDictionary(DataTypeInator* dataTypeInator);
bool addClassFourToTypeDictionary(DataTypeInator* dataTypeInator);
bool addClassFiveToTypeDictionary(DataTypeInator* dataTypeInator);
bool addClassFiveToTypeDictionary(DataTypeInator* dataTypeInator);
bool addClassSixToTypeDictionary(DataTypeInator* dataTypeInator);


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
