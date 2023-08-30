#include <sstream>

#include "io_foo.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "ICGTestFixture.hpp"


TEST_F(ICGTest, inhertance_restore_checkpoint) {
    // ARRANGE
    std::stringstream checkpoint_stream("b.a = 50 ;\nb.c = 42.42 ;\n");

    B b;
    memoryManager.declare_var("B b", &b);

    // ACT
    memoryManager.restore_checkpoint(checkpoint_stream);

    // ASSERT
    EXPECT_EQ(50, b.a);
    EXPECT_EQ(42.42, b.c);
}


TEST_F(ICGTest, inhertance_dump_and_restore_checkpoint) {
    // ARRANGE
    std::stringstream checkpoint_stream;

    A a;
    a.a = 500;
    B b;
    b.a = 50;
    b.c = 42.42;
    memoryManager.declare_var("A a", &a);
    memoryManager.declare_var("B b", &b);

    // write a checkpoint
    memoryManager.write_checkpoint(checkpoint_stream);
    // make some other changes
    a.a = 0;
    b.a = 0;
    b.c = 0.0;

    // ACT
    memoryManager.restore_checkpoint(checkpoint_stream);


    // ASSERT
    EXPECT_EQ(500, a.a);
    EXPECT_EQ(50, b.a);
    EXPECT_EQ(42.42, b.c);
}
