#include <gtest/gtest.h>
#include "MemoryManagement/MemoryManager.hpp"

// This function will be pulled in by whichever io_ file the test includes.
// TODO: maybe instead we have the IO file make the whole DataTypeInator? That might be cool.
// We're gonna have to figure out how to work with multiple IO files
extern void populate_type_dictionary(DataTypeInator *);

class ICGTest : public ::testing::Test {
    public:
    DataTypeInator dataTypeInator;
    MemoryManager memoryManager;


    ICGTest () : memoryManager(&dataTypeInator) {
        populate_type_dictionary(&dataTypeInator);
    }

    ~ICGTest () {}
};