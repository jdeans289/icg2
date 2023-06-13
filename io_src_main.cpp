#include "io_src.hh"
#include "MemoryManagement/MemMgr.hh"
#include "MemoryManagement/AllocInfo.hh"

int main () {

    DataTypeInator dataTypeInator;

    populate_type_dictionary(&dataTypeInator);
    MemMgr mm (&dataTypeInator);

    // std::cout << dictionary.toString() << std::endl;

    

    // if (my_class_instance == NULL) {
    //     std::cout << "Failed to allocate." << std::endl;
    //     return 1;
    // }

    MyClass * my_class_instance = new MyClass;

    my_class_instance->my_int = 5;

    for (int i = 0; i < 5; i++) {
        my_class_instance->my_nested_class.my_arr[i] = i+5;
    }

    (MyClass *) mm.declare_var("MyClass a", my_class_instance);


    AllocInfo * my_class_alloc_info = mm.getAllocInfoNamed("a");
    std::cout << my_class_alloc_info->toString() << std::endl;

    // const DataType * int_datatype = dictionary.getDataType("int");
    // int * my_int_instance = (int *) mm.declare_var("int a");
    // *my_int_instance = 5;


    // AllocInfo * int_alloc_info = mm.getAllocInfoNamed("a");
    // std::cout << int_alloc_info->toString() << std::endl;


    
    
    
}