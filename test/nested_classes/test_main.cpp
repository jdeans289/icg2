#include "io_bar.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "ICGTestFixture.hpp"


TEST_F(ICGTest, nested_classes_checkpoint_restore) {
    // ARRANGE
    std::string checkpoint_str = "Bar b ;\nb.foo.x = 4 ;\nb.foobar.y = 2 ;\nb_ptr = &b ;\n";
    Bar * b_ptr = (Bar *) memoryManager.declare_var("Bar b");
    memoryManager.declare_var("Bar * b_ptr", &b_ptr);

    std::stringstream ss(checkpoint_str);

    // ACT
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    ASSERT_TRUE(b_ptr != NULL);
    ASSERT_EQ(4, b_ptr->foo.x);
    ASSERT_EQ(2, b_ptr->foobar.y);
}

TEST_F(ICGTest, nested_classes_dump_and_restore) {
    // ARRANGE
    Bar * b_ptr = (Bar *) memoryManager.declare_var("Bar b");
    memoryManager.declare_var("Bar * b_ptr", &b_ptr);

    b_ptr->foo.x = 4;
    b_ptr->foobar.y = 2;

    // dump a checkpoint
    std::stringstream ss;
    memoryManager.write_checkpoint(ss);

    // Change it to something else
    b_ptr->foo.x = 400;
    b_ptr->foobar.y = 200;

    // Restore the first checkpoint
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    ASSERT_TRUE(b_ptr != NULL);
    ASSERT_EQ(4, b_ptr->foo.x);
    ASSERT_EQ(2, b_ptr->foobar.y);
}



