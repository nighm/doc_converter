#include <gtest/gtest.h>

// 基本的测试用例
TEST(BasicTest, AlwaysTrue) {
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 