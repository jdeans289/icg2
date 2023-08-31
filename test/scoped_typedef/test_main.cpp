#include "io_foo.hpp"
#include "MemoryManagement/AllocInfo.hpp"

#include "ICGTestFixture.hpp"


// TEST_F(ICGTest, scoped_typedef_test) {
//     Jackie::JString * js = (Jackie::JString *) memoryManager.declare_var("Jackie::JString js");
//     StrVec * v = (StrVec *) memoryManager.declare_var("StrVec v");
//     A::MyOtherString * a = (A::MyOtherString *) memoryManager.declare_var("A::MyOtherString a");

//     *js = "JackieString";
//     *v = {"this", "is", "a", "vec"};
//     *a = "another stupid string";

//     memoryManager.write_checkpoint(std::cout);
// }

TEST_F(ICGTest, typedef_restore_test) {
    // ARRANGE
    std::string checkpoint_str = R"(
RESIZE_STL v.size = 4 ;
v[0] = "this" ;
v[1] = "is" ;
v[2] = "a" ;
v[3] = "vec" ;
a = "another stupid string" ;
js = "JackieString" ;
)";
    std::stringstream ss(checkpoint_str);

    Jackie::JString js;
    memoryManager.declare_var("Jackie::JString js", &js);
    StrVec v;
    memoryManager.declare_var("StrVec v", &v);
    A::MyOtherString a;
    memoryManager.declare_var("A::MyOtherString a", &a);

    // ACT
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    EXPECT_EQ(std::string("JackieString"), js);
    EXPECT_EQ(std::string("another stupid string"), a);
    std::vector<std::string> expected = {"this", "is", "a", "vec"};
    EXPECT_EQ(expected, v);
}

TEST_F(ICGTest, typedef_dump_and_restore_test) {
    // ARRANGE
    Jackie::JString js;
    memoryManager.declare_var("Jackie::JString js", &js);
    StrVec v;
    memoryManager.declare_var("StrVec v", &v);
    A::MyOtherString a;
    memoryManager.declare_var("A::MyOtherString a", &a);

    js = "JackieString";
    v = {"this", "is", "a", "vec"};
    a = "another stupid string";

    std::stringstream ss;
    // dump a checkpoint
    memoryManager.write_checkpoint(ss);

    // change vars
    js = "";
    v = {};
    a = "";

    // ACT
    memoryManager.restore_checkpoint(ss);

    // ASSERT
    EXPECT_EQ(std::string("JackieString"), js);
    EXPECT_EQ(std::string("another stupid string"), a);
    std::vector<std::string> expected = {"this", "is", "a", "vec"};
    EXPECT_EQ(expected, v);
}