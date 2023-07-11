#include "io_header.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "ICGTestFixture.hpp"


TEST_F(ICGTest, CheckpointRestore) {
    // ARRANGE
    MyClass * my_class_instance;
    my_class_instance = (MyClass *) memoryManager.declare_var("MyClass a");
    memoryManager.declare_var("MyClass * ptr", &my_class_instance);
    
    // ACT
    memoryManager.restore_checkpoint("checkpoint.txt");

    // ASSERT
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(my_class_instance->my_nested_class.my_arr[i], i+5);
    }
}