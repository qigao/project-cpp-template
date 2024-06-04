#ifndef _CSV_READER_TEST_H_
#define _CSV_READER_TEST_H_

#include <fstream>
#include <iostream>

#include <gtest/gtest.h>

#include "csv_record_parser.h"
#include "csv_stream_parser.h"
#include "error.h"
#include "input_reader.h"
class CSVReaderTest : public ::testing::Test
{
};

// should not work in any way...
TEST_F(CSVReaderTest, TestEmptyFile)
{
    auto file_stream = std::make_unique<std::ifstream>(
        "./fixtures/empty.csv", std::ios_base::in | std::ios_base::binary);
    auto reader = Parser::CSVStreamParser<>(
        std::make_unique<Parser::StreamReader>(*file_stream));

    ASSERT_FALSE(reader.valid()); // nothing to read...
    ASSERT_TRUE(reader.end());
}

TEST_F(CSVReaderTest, TestSimpleCSV)
{
    auto file_stream = std::make_unique<std::ifstream>(
        "./fixtures/simple.csv", std::ios_base::in | std::ios_base::binary);
    auto reader = Parser::CSVStreamParser<>(
        std::make_unique<Parser::StreamReader>(*file_stream));

    ASSERT_FALSE(reader.end());
    ASSERT_TRUE(reader.valid()); // nothing to read...

    std::vector<std::string> results;
    reader.readLine(results);

    ASSERT_EQ(results[0], "a");
    ASSERT_EQ(results[1], "b");
    ASSERT_EQ(results[2], "c");
    ASSERT_EQ(results[3], "d");

    ASSERT_FALSE(reader.end());
    ASSERT_TRUE(reader.valid());
    results.clear();
    reader.readLine(results);

    ASSERT_EQ(results[0], "e");
    ASSERT_EQ(results[1], "f");
    ASSERT_EQ(results[2], "g");
    ASSERT_EQ(results[3], "h");

    ASSERT_FALSE(reader.valid());
    ASSERT_TRUE(reader.end());
}

TEST_F(CSVReaderTest, TestQuotedCSV)
{
    auto file_stream = std::make_unique<std::ifstream>(
        "./fixtures/simple_quoted.csv",
        std::ios_base::in | std::ios_base::binary);
    auto reader = Parser::CSVStreamParser<>(
        std::make_unique<Parser::StreamReader>(*file_stream));

    ASSERT_FALSE(reader.end());
    ASSERT_TRUE(reader.valid()); // nothing to read...

    std::vector<std::string> results;
    reader.readLine(results);

    ASSERT_EQ(results[0], "a");
    ASSERT_EQ(results[1], "b");
    ASSERT_EQ(results[2], "c, d");

    ASSERT_FALSE(reader.end());
    ASSERT_TRUE(reader.valid());
    results.clear();
    reader.readLine(results);

    ASSERT_EQ(results[0], "e");
    ASSERT_EQ(results[1], "f\"g");
    ASSERT_EQ(results[2], "h");

    ASSERT_FALSE(reader.valid());
    ASSERT_TRUE(reader.end());
}

TEST_F(CSVReaderTest, TestInvalidCSV)
{
    auto file_stream = std::make_unique<std::ifstream>(
        "./fixtures/invalid.csv", std::ios_base::in | std::ios_base::binary);
    auto reader = Parser::CSVStreamParser<>(
        std::make_unique<Parser::StreamReader>(*file_stream));

    ASSERT_FALSE(reader.end());
    ASSERT_TRUE(reader.valid()); // nothing to read...

    std::vector<std::string> results;

    ASSERT_FALSE(reader.end());
    ASSERT_TRUE(reader.valid());
    ASSERT_THROW({ reader.readLine(results); }, Parser::file_parsing_error);
    ASSERT_FALSE(reader.valid());
    ASSERT_FALSE(reader.end());
}

TEST_F(CSVReaderTest, TestMultipleBlocksValid)
{
    auto file_stream = std::make_unique<std::ifstream>(
        "./fixtures/short_block.csv",
        std::ios_base::in | std::ios_base::binary);
    auto reader = Parser::CSVStreamParser<32>(
        std::make_unique<Parser::StreamReader>(*file_stream));

    ASSERT_FALSE(reader.end());
    ASSERT_TRUE(reader.valid()); // nothing to read...

    std::vector<std::string> results;
    reader.readLine(results);

    ASSERT_EQ(results[0], "Carrot");
    ASSERT_EQ(results[1], "Cabbage");
    ASSERT_EQ(results[2], "Cauliflower");
    ASSERT_EQ(results[3], "Peas");

    ASSERT_FALSE(reader.end());
    ASSERT_TRUE(reader.valid());
    results.clear();
    reader.readLine(results);

    ASSERT_EQ(results[0], "Pineapple");
    ASSERT_EQ(results[1], "Pear");
    ASSERT_EQ(results[2], "Peach");
    ASSERT_EQ(results[3], "Prune");

    ASSERT_FALSE(reader.end());
    ASSERT_TRUE(reader.valid());
    results.clear();
    reader.readLine(results);

    ASSERT_EQ(results[0], "Switzerland");
    ASSERT_EQ(results[1], "Sweden");
    ASSERT_EQ(results[2], "Germany");
    ASSERT_EQ(results[3], "Spain");

    ASSERT_TRUE(reader.end());
    ASSERT_FALSE(reader.valid());
}

TEST_F(CSVReaderTest, TestMultipleBlocksInvalid)
{
    auto file_stream = std::make_unique<std::ifstream>(
        "./fixtures/short_block.csv",
        std::ios_base::in | std::ios_base::binary);
    auto reader = Parser::CSVStreamParser<16>(
        std::make_unique<Parser::StreamReader>(*file_stream));

    ASSERT_TRUE(reader.valid()); // nothing to read...

    // the first works
    std::vector<std::string> results;
    reader.readLine(results);

    ASSERT_EQ(results[0], "Carrot");
    ASSERT_EQ(results[1], "Cabbage");
    ASSERT_EQ(results[2], "Cauliflower");
    ASSERT_EQ(results[3], "Peas");

    ASSERT_FALSE(reader.end());
    ASSERT_TRUE(reader.valid());
    results.clear();
    // the second does not
    ASSERT_THROW({ reader.readLine(results); }, Parser::file_parsing_error);
    ASSERT_FALSE(reader.valid());
    ASSERT_FALSE(reader.end());
}

#endif
