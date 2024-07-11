#include "human_readable.hpp"

#include <gtest/gtest.h>
constexpr size_t IntegerRepresentableBoundary()
{
    return size_t{2} << (std::numeric_limits<double>::digits - 1);
}
TEST(HumanReadableTest, kilo)
{
    HumanReadable hr;
    hr.size = 1024;

    std::ostringstream oss;
    oss << hr;

    EXPECT_EQ(oss.str(), "1KB (1024)");
}
TEST(HumanReadableTest, streamcast)
{
    EXPECT_EQ(static_cast<size_t>(NarrowCast<double>(
                  size_t{IntegerRepresentableBoundary() - 2})),
              size_t{IntegerRepresentableBoundary() - 2});
    EXPECT_EQ(static_cast<size_t>(NarrowCast<double>(
                  size_t{IntegerRepresentableBoundary() - 1})),
              size_t{IntegerRepresentableBoundary() - 1});
    EXPECT_EQ(static_cast<size_t>(
                  NarrowCast<double>(size_t{IntegerRepresentableBoundary()})),
              size_t{IntegerRepresentableBoundary()});
    EXPECT_THROW(NarrowCast<double>(size_t{IntegerRepresentableBoundary() + 1}),
                 std::exception);
    EXPECT_EQ(static_cast<size_t>(NarrowCast<double>(
                  size_t{IntegerRepresentableBoundary() + 2})),
              size_t{IntegerRepresentableBoundary() + 2});
    EXPECT_THROW(NarrowCast<double>(size_t{IntegerRepresentableBoundary() + 3}),
                 std::exception);
    EXPECT_EQ(static_cast<size_t>(NarrowCast<double>(
                  size_t{IntegerRepresentableBoundary() + 4})),
              size_t{IntegerRepresentableBoundary() + 4});
    EXPECT_THROW(NarrowCast<double>(size_t{IntegerRepresentableBoundary() + 5}),
                 std::exception);
}
