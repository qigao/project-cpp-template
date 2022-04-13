#include "socket/utils.hpp"
#include <gtest/gtest.h>

TEST(utils, ifaces)
{
  auto lists = get_interface_list();
  ASSERT_GT(lists.size(), 0);
}

TEST(utils, cpu_info)
{
  auto info = get_cpu_info();
  ASSERT_GT(info["size"].size(), 0);
  ASSERT_GT(info["model"].size(), 0);
  ASSERT_GT(info["speed"].size(), 0);
}

TEST(utils, load_average)
{
  auto info = get_load_average();
  ASSERT_GT(info.size(), 0);
}

TEST(utils, total_memory)
{
  auto info = get_total_memory();
  ASSERT_GT(info, 0);
}

TEST(utils, up_time)
{
  auto info = get_up_time();
  ASSERT_GT(info, 0);
}

TEST(utils, hr_time)
{
  auto info = get_hr_time();
  ASSERT_GT(info, 0);
}

TEST(utils, path)
{
  auto info = get_path();
  ASSERT_GT(info.size(), 0);
}

TEST(utils, cwd)
{
  auto info = get_cwd();
  ASSERT_GT(info.size(), 0);
}

TEST(utils, rusage) { ASSERT_NO_THROW(get_rusage()); }
