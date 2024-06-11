#include "CSVParser.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <string>
using ::testing::Contains;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;
using namespace Parser;
class CSVParserTest : public ::testing::Test
{
};

TEST_F(CSVParserTest, TestEmpyFile)
{
    auto parser = std::make_unique<CSVParser>();
    auto result = parser->parse("./fixtures/empty.csv");
    ASSERT_TRUE(result.empty());
}

TEST_F(CSVParserTest, TestSimpleCSV)
{
    auto parser = std::make_unique<CSVParser>();
    auto result = parser->parse("./fixtures/simple.csv");
    ASSERT_THAT(result,
                UnorderedElementsAre(UnorderedElementsAre("a", "b", "c"),
                                     UnorderedElementsAre("d", "e", "f")));
}
TEST_F(CSVParserTest, TestQuotedCSV)
{
    auto parser = std::make_unique<CSVParser>();
    auto result = parser->parse("./fixtures/simple_quoted.csv");
    ASSERT_EQ(result.size(), 2);
    ASSERT_THAT(result,
                UnorderedElementsAre(UnorderedElementsAre("a", "b", "c"),
                                     UnorderedElementsAre("d", "e", "f")));
}
TEST_F(CSVParserTest, TestInvalidCSV)
{
    auto parser = std::make_unique<CSVParser>();
    auto result = parser->parse("./fixtures/invalid.csv");
    ASSERT_EQ(result.size(), 2);
    ASSERT_THAT(result,
                UnorderedElementsAre(UnorderedElementsAre("a", "b", "c"),
                                     UnorderedElementsAre("d", "e", "f")));
}
TEST_F(CSVParserTest, TestMultipleBlocksInvalid)
{
    auto parser = std::make_unique<CSVParser>();
    auto result = parser->parse("./fixtures/short_block.csv");
    ASSERT_EQ(result.size(), 2);
    ASSERT_THAT(result,
                UnorderedElementsAre(UnorderedElementsAre("a", "b", "c"),
                                     UnorderedElementsAre("d", "e", "f")));
}
