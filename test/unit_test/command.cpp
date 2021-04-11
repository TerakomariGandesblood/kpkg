#include <gtest/gtest.h>

#include "command.h"

TEST(CommandTest, Combine) {
  EXPECT_EQ(kpkg::detail::combine_cmd("a", "b"), "a && b");
  EXPECT_EQ(kpkg::detail::combine_cmd("", "b"), "b");
  EXPECT_EQ(kpkg::detail::combine_cmd("a", ""), "a");
  EXPECT_TRUE(std::empty(kpkg::detail::combine_cmd("", "")));
}

TEST(CommandTest, DealWithBoost) {
  EXPECT_EQ(kpkg::detail::deal_with_boost(
                {"./bootstrap.sh", "none", "memory", "thread"}, "boost/???",
                kpkg::Sanitize::None),
            "cd boost/??? && " + kpkg::detail::export_gcc +
                " && ./bootstrap.sh && none");

  EXPECT_EQ(kpkg::detail::deal_with_boost(
                {"./bootstrap.sh", "none", "memory", "thread"}, "boost",
                kpkg::Sanitize::Memory),
            "cd boost && " + kpkg::detail::export_clang +
                " && ./bootstrap.sh && memory");

  EXPECT_EQ(kpkg::detail::deal_with_boost(
                {"./bootstrap.sh", "none", "memory", "thread"}, "boost",
                kpkg::Sanitize::Thread),
            "cd boost && " + kpkg::detail::export_clang +
                " && ./bootstrap.sh && thread");
}

TEST(CommandTest, CalcCmd) {
  EXPECT_EQ(kpkg::detail::calc_cmd({"configure", "build", "install"}, "cwd",
                                   kpkg::Sanitize::None),
            "cd cwd && " + kpkg::detail::export_gcc + " && " +
                kpkg::detail::export_flag +
                " && configure && build && install");

  EXPECT_EQ(kpkg::detail::calc_cmd({"configure", "build", "install"}, "cwd",
                                   kpkg::Sanitize::Memory),
            "cd cwd && " + kpkg::detail::export_clang + " && " +
                kpkg::detail::export_memory_flag +
                " && configure && build && install");

  EXPECT_EQ(kpkg::detail::calc_cmd({"configure", "build", "install"}, "cwd",
                                   kpkg::Sanitize::Thread),
            "cd cwd && " + kpkg::detail::export_clang + " && " +
                kpkg::detail::export_thread_flag +
                " && configure && build && install");
}
