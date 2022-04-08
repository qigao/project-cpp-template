#include "socket/utils.hpp"
#include <gtest/gtest.h>

TEST(uvw, ifaces)
{
  auto lists = get_interface_list();
  ASSERT_GT(lists.size(), 0);
}

TEST(uvw, cpu_info)
{
  auto info = get_cpu_info();
  ASSERT_GT(info["size"].size(), 0);
  ASSERT_GT(info["model"].size(), 0);
  ASSERT_GT(info["speed"].size(), 0);
}

TEST(uvw, load_average)
{
  auto info = get_load_average();
  ASSERT_GT(info.size(), 0);
}

TEST(uvw, total_memory)
{
  auto info = get_total_memory();
  ASSERT_GT(info, 0);
}

TEST(uvw, up_time)
{
  auto info = get_up_time();
  ASSERT_GT(info, 0);
}

TEST(uvw, hr_time)
{
  auto info = get_hr_time();
  ASSERT_GT(info, 0);
}

TEST(uvw, path)
{
  auto info = get_path();
  ASSERT_GT(info.size(), 0);
}

TEST(uvw, cwd)
{
  auto info = get_cwd();
  ASSERT_GT(info.size(), 0);
}

TEST(uvw, rusage) { ASSERT_NO_THROW(get_rusage()); }
