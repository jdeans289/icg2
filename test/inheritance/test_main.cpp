#include <sstream>

#include "io_foo.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "ICGTestFixture.hpp"


TEST_F(ICGTest, inhertance_restore_checkpoint) {
    // ARRANGE
    std::stringstream checkpoint_stream("b.a = 50 ;\nb.b = 42.42 ;\n");

    B b;
    memoryManager.declare_var("B b", &b);

    // ACT
    memoryManager.restore_checkpoint(checkpoint_stream);

    // ASSERT
    EXPECT_EQ(50, b.a);
    EXPECT_EQ(42.42, b.b);
}


TEST_F(ICGTest, inhertance_dump_and_restore_checkpoint) {
    // ARRANGE
    std::stringstream checkpoint_stream;

    A a;
    a.a = 500;
    B b;
    b.a = 50;
    b.b = 42.42;
    C c;
    c.a = 5;
    c.b = 4.2;
    c.c = "Hello";
    memoryManager.declare_var("A a", &a);
    memoryManager.declare_var("B b", &b);
    memoryManager.declare_var("C c", &c);

    // write a checkpoint
    memoryManager.write_checkpoint(checkpoint_stream);
    // make some other changes
    a.a = 0;
    b.a = 0;
    b.b = 0.0;
    c.a = 0;
    c.b = 0.0;
    c.c = "Goodbye";

    // ACT
    memoryManager.restore_checkpoint(checkpoint_stream);


    // ASSERT
    EXPECT_EQ(500, a.a);
    EXPECT_EQ(50, b.a);
    EXPECT_EQ(42.42, b.b);
    EXPECT_EQ(5, c.a);
    EXPECT_EQ(4.2, c.b);
    EXPECT_EQ(std::string("Hello"), c.c);

}
