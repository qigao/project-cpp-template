//
//  Test.Encoding.cpp
//  This file is part of the "URI" project and released under the MIT License.
//
//  Created by Samuel Williams on 17/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <URI/Encoding.hpp>
#include <UnitTest/UnitTest.hpp>
#include <map>

namespace URI
{
UnitTest::Suite EncodingTestSuite{
    "URI::Encoding",

    {"it can percent encode non-pchars", [](UnitTest::Examiner& examiner)
     { examiner.expect(Encoding::encode("foo bar")) == "foo%20bar"; }},

    {"it can percent decode", [](UnitTest::Examiner& examiner)
     { examiner.expect(Encoding::decode("foo%20bar")) == "foo bar"; }},

    {"it can encode query",
     [](UnitTest::Examiner& examiner)
     {
         std::map<std::string, std::string> arguments = {
             {"x[]", "http://www.google.com/search?q=lucid"}};

         auto query =
             Encoding::encode_query(arguments.begin(), arguments.end());

         examiner.expect(query) ==
             "x[]=http://www.google.com/search%3Fq%3Dlucid";
     }},

    {"it doesn't encode unreserved characters",
     [](UnitTest::Examiner& examiner)
     {
         std::map<std::string, std::string> arguments = {
             {"clip", "{{1,2},{3,4}}"}};

         auto query =
             Encoding::encode_query(arguments.begin(), arguments.end());

         examiner.expect(query) == "clip={{1,2},{3,4}}";
     }},

    {"it can encode path",
     [](UnitTest::Examiner& examiner)
     {
         std::vector<std::string> entries = {
             "blog", "2017", "Apples/oranges & the path to fruit salad!"};

         auto path = Encoding::encode_path(entries.begin(), entries.end());

         examiner.expect(path) ==
             "blog/2017/"
             "Apples%2Foranges%20&%20the%20path%20to%20fruit%20salad!";
     }},

    {"it can encode native path",
     [](UnitTest::Examiner& examiner)
     {
         auto encoded = Encoding::encode_path("foo:bar", ':', true);

         examiner.expect(encoded) == "foo/bar/";
     }},

    {"it can encode native path containing utf8",
     [](UnitTest::Examiner& examiner)
     {
         auto encoded = Encoding::encode_path("I/♥/You", '/', false);

         examiner.expect(encoded) == "I/%E2%99%A5/You";

         auto decoded = Encoding::decode_path(encoded, '/');

         examiner.expect(decoded) == "I/♥/You";
     }},
};
}
