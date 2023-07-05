#include "io_src.hh"
#include "MemoryManagement/MemMgr.hh"
#include "MemoryManagement/AllocInfo.hh"

int main () {

    DataTypeInator dataTypeInator;

    populate_type_dictionary(&dataTypeInator);
    MemMgr mm (&dataTypeInator);

    MyClass * my_class_instance;
    my_class_instance = (MyClass *) mm.declare_var("MyClass a");
    mm.declare_var("MyClass * ptr", &my_class_instance);
    
    mm.restore_checkpoint("checkpoint_filename.txt");
    for (int i = 0; i < 5; i++) {
        std::cout << "my_class_instance->my_nested_class.arr[" << i << "] = " << my_class_instance->my_nested_class.my_arr[i] << std::endl;
    }
}