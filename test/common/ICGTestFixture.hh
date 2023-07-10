#include <gtest/gtest.h>
#include "MemoryManagement/MemMgr.hh"


class ICGTest : public ::testing::Test {
    public:
    DataTypeInator dataTypeInator;
    MemMgr memoryManager;


    ICGTest () : memoryManager(&dataTypeInator) {
        populate_type_dictionary(&dataTypeInator);
    }

    ~ICGTest () {}
};