#include <sstream>

#include "io_Foo.hpp"

#include "ICGTestFixture.hpp"


TEST_F(ICGTest, example_restore) {
    // ARRANGE
    Foo foo;
    memoryManager.declare_var("Foo foo", &foo);

    std::stringstream checkpoint_stream("RESIZE_STL foo.b.c = 10;\nfoo.b.c[5] = \"Hello Trick!\"\n");

    // ACT
    memoryManager.restore_checkpoint(checkpoint_stream);

    // ASSERT
    EXPECT_EQ(std::string("Hello Trick!"), foo.b.c[5]);
}

