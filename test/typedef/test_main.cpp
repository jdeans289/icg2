#include "io_foo.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "ICGTestFixture.hpp"


TEST_F(ICGTest, typedef_restore_test) {
    // ARRANGE
    std::string checkpoint_str = R"(
// Variable Assignments.
a.x = "Hello hello" ;
my_str = "Aliased type!" ;
)";
    std::stringstream ss(checkpoint_str);

    A a;
    memoryManager.declare_var("A a", &a);


    MyString my_str;
    memoryManager.declare_var("MyString my_str", &my_str);

    // ACT
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    EXPECT_EQ(std::string("Hello hello"), a.x);
    EXPECT_EQ(std::string("Aliased type!"), my_str);
}

TEST_F(ICGTest, typedef_dump_and_restore_test) {
    // ARRANGE
    A a;
    memoryManager.declare_var("A a", &a);
    MyString my_str;
    memoryManager.declare_var("MyString my_str", &my_str);

    a.x = "Hello hello" ;
    my_str = "Aliased type!" ;

    // dump a checkpoint
    std::stringstream ss;
    memoryManager.write_checkpoint(ss);

    // clear the vars
    a.x = "";
    my_str = "";

    // ACT
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    EXPECT_EQ(std::string("Hello hello"), a.x);
    EXPECT_EQ(std::string("Aliased type!"), my_str);
}