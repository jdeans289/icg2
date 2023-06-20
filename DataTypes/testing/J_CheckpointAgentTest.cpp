// Test support
#include "DataTypeTestSupport.hh"
#include <sstream>
#include <gtest/gtest.h>

// Class under test
#include "J_CheckpointAgent.hh"


class J_CheckpointAgentTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;

    J_CheckpointAgentTest() {

    }

    ~J_CheckpointAgentTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

TEST_F (J_CheckpointAgentTest, whole_format_basic_local) {
    // ARRANGE
    AllocInfo my_int_info ("my_int", "int", &dataTypeInator);
    int * my_int = (int *) my_int_info.getStart();
    *my_int = 42;

    J_CheckpointAgent checkpoint_agent;
    std::stringstream ss;
    std::vector<AllocInfo *> alloc_list;
    alloc_list.push_back(&my_int_info);

    // ACT
    checkpoint_agent.dump(ss, alloc_list);
    
    // std::cout << "Result: " << std::endl;
    // std::cout << ss.str() << std::endl;
    
    // ASSERT

    // I don't love this way that I'm testing
    std::string expected(R"(// Variable Declarations.
int my_int ;


// Clear all allocations to 0.
clear_all_vars();


// Variable Assignments.
my_int = 42 ;
)");

    EXPECT_EQ(expected, ss.str());

}

TEST_F (J_CheckpointAgentTest, whole_format_basic_extern) {
    // ARRANGE
    int my_int = 42;
    AllocInfo my_int_info ("my_int", "int", &dataTypeInator, &my_int);

    J_CheckpointAgent checkpoint_agent;
    std::stringstream ss;
    std::vector<AllocInfo *> alloc_list;
    alloc_list.push_back(&my_int_info);

    // ACT
    checkpoint_agent.dump(ss, alloc_list);
    
    // std::cout << "Result: " << std::endl;
    // std::cout << ss.str() << std::endl;

std::string expected(R"(// Variable Declarations.


// Clear all allocations to 0.
clear_all_vars();


// Variable Assignments.
my_int = 42 ;
)");

    EXPECT_EQ(expected, ss.str());

}

TEST_F (J_CheckpointAgentTest, pointer) {
    // ARRANGE
    
    // vars to checkpoint
    int my_int = 42;
    int * my_int_ptr = &my_int;

    AllocInfo my_int_info ("my_int", "int", &dataTypeInator, &my_int);
    AllocInfo my_int_ptr_info ("my_int_ptr", "int*", &dataTypeInator, &my_int_ptr);

    J_CheckpointAgent checkpoint_agent;
    std::stringstream ss;
    std::vector<AllocInfo *> alloc_list;
    alloc_list.push_back(&my_int_info);
    alloc_list.push_back(&my_int_ptr_info);

    // ACT
    checkpoint_agent.writeVariable(ss, &my_int_ptr_info, alloc_list);

    std::string expected("my_int_ptr = &my_int ;\n");

    EXPECT_EQ(expected, ss.str());
}

TEST_F (J_CheckpointAgentTest, classes) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    // vars to checkpoint
    ClassTwo var_to_checkpoint = {.x = 100, .y = 5.5, .c1 = {.a = 5, .b = 1.5}};

    AllocInfo my_alloc_info ("var_to_checkpoint", "ClassTwo", &dataTypeInator, &var_to_checkpoint);

    J_CheckpointAgent checkpoint_agent;
    std::stringstream ss;
    std::vector<AllocInfo *> alloc_list;
    alloc_list.push_back(&my_alloc_info);

    // ACT
    checkpoint_agent.writeVariable(ss, &my_alloc_info, alloc_list);
    
    // std::cout << "Result: " << std::endl;
    // std::cout << ss.str() << std::endl;

    std::string expected(R"(var_to_checkpoint.x = 100 ;
var_to_checkpoint.y = 5.5 ;
var_to_checkpoint.c1.a = 5 ;
var_to_checkpoint.c1.b = 1.5 ;
)");

    EXPECT_EQ(expected, ss.str());
}

TEST_F (J_CheckpointAgentTest, whole_format_pointers_galore) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    std::vector<AllocInfo *> alloc_list;
    
    // vars to checkpoint
    ClassTwo my_class = {.x = 100, .y = 5.5, .c1 = {.a = 5, .b = 1.5}};
    AllocInfo my_class_alloc ("var_to_checkpoint", "ClassTwo", &dataTypeInator, &my_class);
    alloc_list.push_back(&my_class_alloc);

    int my_arr[5] = {1, 2, 3, 4, 5};
    AllocInfo my_arr_alloc ("var_to_checkpoint", "int[5]", &dataTypeInator, &my_arr);
    alloc_list.push_back(&my_arr_alloc);

    int * my_arr_3_ptr = &my_arr[3];
    AllocInfo my_arr_3_ptr_alloc ("my_arr_3_ptr", "int *", &dataTypeInator, &my_arr_3_ptr);
    alloc_list.push_back(&my_arr_3_ptr_alloc);

    int (*my_arr_ptr)[5] = &my_arr;
    AllocInfo my_arr_ptr_alloc ("my_arr_ptr", "int (*)[5]", &dataTypeInator, &my_arr_ptr);
    alloc_list.push_back(&my_arr_ptr_alloc);

    int * x_ptr = &my_class.x;
    AllocInfo x_alloc ( "x_ptr", "int *", &dataTypeInator, &x_ptr );
    alloc_list.push_back(&x_alloc);

    double * y_ptr = &my_class.y;
    AllocInfo y_alloc ( "y_ptr", "double *", &dataTypeInator, &y_ptr );
    alloc_list.push_back(&y_alloc);

    ClassOne * c1_ptr = &my_class.c1;
    AllocInfo c1_ptr_alloc ( "c1_ptr", "ClassOne *", &dataTypeInator, &c1_ptr );
    alloc_list.push_back(&c1_ptr_alloc);

    int * a_ptr = &my_class.c1.a;
    AllocInfo a_ptr_alloc ( "a_ptr", "int *", &dataTypeInator, &a_ptr );
    alloc_list.push_back(&a_ptr_alloc);

    double * b_ptr = &my_class.c1.b;
    AllocInfo b_ptr_alloc ( "b_ptr", "double *", &dataTypeInator, &b_ptr );
    alloc_list.push_back(&b_ptr_alloc);

    J_CheckpointAgent checkpoint_agent;
    std::stringstream ss;

    // ACT
    checkpoint_agent.dump(ss, alloc_list);
    
    // std::cout << "Result: " << std::endl;
    // std::cout << ss.str() << std::endl;

    std::string expected(R"(// Variable Declarations.


// Clear all allocations to 0.
clear_all_vars();


// Variable Assignments.
var_to_checkpoint.x = 100 ;
var_to_checkpoint.y = 5.5 ;
var_to_checkpoint.c1.a = 5 ;
var_to_checkpoint.c1.b = 1.5 ;
var_to_checkpoint[0] = 1 ;
var_to_checkpoint[1] = 2 ;
var_to_checkpoint[2] = 3 ;
var_to_checkpoint[3] = 4 ;
var_to_checkpoint[4] = 5 ;
my_arr_3_ptr = &var_to_checkpoint[3] ;
my_arr_ptr = &var_to_checkpoint ;
x_ptr = &var_to_checkpoint.x ;
y_ptr = &var_to_checkpoint.y ;
c1_ptr = &var_to_checkpoint.c1 ;
a_ptr = &var_to_checkpoint.c1.a ;
b_ptr = &var_to_checkpoint.c1.b ;
)");

    EXPECT_EQ(expected, ss.str());
}