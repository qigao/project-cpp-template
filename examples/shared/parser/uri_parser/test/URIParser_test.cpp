#include "URIParser.h"
#include "uri_values.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

using ::testing::Contains;
using ::testing::Pair;
using ::testing::UnorderedElementsAre;
using namespace Parser;
TEST(URIParser, ExampleURI)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri = std::string("http://foo@example.com:81/a/b/c?x=7&y=11#hola");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);
    EXPECT_THAT(values, UnorderedElementsAre(
                            Pair("schema", "http"), Pair("host", "example.com"),
                            Pair("port", "81"), Pair("path", "/a/b/c"),
                            Pair("userinfo", "foo"), Pair("query", "x=7&y=11"),
                            Pair("frag", "hola")));
}
TEST(URIParser, IPV4URI)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri = std::string("http://foo@192.168.2.2:81/a/b/c?x=7&y=11#hola");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);

    EXPECT_THAT(values, UnorderedElementsAre(
                            Pair("schema", "http"), Pair("host", "192.168.2.2"),
                            Pair("port", "81"), Pair("path", "/a/b/c"),
                            Pair("userinfo", "foo"), Pair("query", "x=7&y=11"),
                            Pair("frag", "hola")));
}
TEST(URIParser, IPV6URI)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri = std::string(
        "http://foo@[2001:db8::8a2e:370:7334]:81/a/b/c?x=7&y=11#hola");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);

    EXPECT_THAT(values, UnorderedElementsAre(
                            Pair("schema", "http"),
                            Pair("host", "[2001:db8::8a2e:370:7334]"),
                            Pair("port", "81"), Pair("path", "/a/b/c"),
                            Pair("userinfo", "foo"), Pair("query", "x=7&y=11"),
                            Pair("frag", "hola")));
}
TEST(URIParser, ExampleURI_WithoutFrag)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri = std::string("http://foo@example.com:81/a/b/c?x=7&y=11");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);

    EXPECT_THAT(values,
                UnorderedElementsAre(
                    Pair("schema", "http"), Pair("host", "example.com"),
                    Pair("port", "81"), Pair("path", "/a/b/c"),
                    Pair("userinfo", "foo"), Pair("query", "x=7&y=11")));
}

TEST(URIParser, IPV6URI_WithoutFrag)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri =
        std::string("http://foo@[2001:db8::8a2e:370:7334]:81/a/b/c?x=7&y=11");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);

    EXPECT_THAT(values,
                UnorderedElementsAre(Pair("schema", "http"),
                                     Pair("host", "[2001:db8::8a2e:370:7334]"),
                                     Pair("port", "81"), Pair("path", "/a/b/c"),
                                     Pair("userinfo", "foo"),
                                     Pair("query", "x=7&y=11")));
}

TEST(URIParser, IPV4URI_WithoutFrag)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri = std::string("http://foo@192.168.2.1:81/a/b/c?x=7&y=11");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);

    EXPECT_THAT(values,
                UnorderedElementsAre(
                    Pair("schema", "http"), Pair("host", "192.168.2.1"),
                    Pair("port", "81"), Pair("path", "/a/b/c"),
                    Pair("userinfo", "foo"), Pair("query", "x=7&y=11")));
}
TEST(URIParser, IPV6URI_WithoutUser)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri =
        std::string("http://[2001:db8::8a2e:370:7334]:81/a/b/c?x=7&y=11");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);

    EXPECT_THAT(values,
                UnorderedElementsAre(Pair("schema", "http"),
                                     Pair("host", "[2001:db8::8a2e:370:7334]"),
                                     Pair("port", "81"), Pair("path", "/a/b/c"),
                                     Pair("query", "x=7&y=11")));
}
TEST(URIParser, ExampleURI_WithoutUser)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri = std::string("http://example.com:81/a/b/c?x=7&y=11");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);

    EXPECT_THAT(values, UnorderedElementsAre(
                            Pair("schema", "http"), Pair("host", "example.com"),
                            Pair("port", "81"), Pair("path", "/a/b/c"),
                            Pair("query", "x=7&y=11")));
}
TEST(URIParser, IPV4URI_WithoutUser)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri = std::string("http://192.168.2.1:81/a/b/c?x=7&y=11");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);

    EXPECT_THAT(values, UnorderedElementsAre(
                            Pair("schema", "http"), Pair("host", "192.168.2.1"),
                            Pair("port", "81"), Pair("path", "/a/b/c"),
                            Pair("query", "x=7&y=11")));
}
TEST(URIParser, ExampleURI_WithFrag)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri = std::string("http://example.com:81/a/b/c?x=7&y=11#hola");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);

    EXPECT_THAT(values, UnorderedElementsAre(
                            Pair("schema", "http"), Pair("host", "example.com"),
                            Pair("port", "81"), Pair("path", "/a/b/c"),
                            Pair("frag", "hola"), Pair("query", "x=7&y=11")));
}
TEST(URIParser, IPV4URI_WithFrag)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri = std::string("http://192.168.2.1:81/a/b/c?x=7&y=11#hola");
    parser->execute(sip_uri.data(), sip_uri.length(), 0, values);

    EXPECT_THAT(values, UnorderedElementsAre(
                            Pair("schema", "http"), Pair("host", "192.168.2.1"),
                            Pair("port", "81"), Pair("path", "/a/b/c"),
                            Pair("frag", "hola"), Pair("query", "x=7&y=11")));
}

TEST(URIParser, SIPURI)
{
    NamedValues values;

    auto parser = std::make_unique<URIParser>();
    auto sip_uri = std::string(
        "sips:alice@gateway.com:5061;transport=udp;user=phone;method="
        "REGISTER?subject=sales%20meeting&priority=urgent&to=sales%40city.com");
    parser->execute(sip_uri.data(), sip_uri.length() + 1, 0, values);

    EXPECT_THAT(values,
                UnorderedElementsAre(
                    Pair("host", "gateway.com"), Pair("schema", "sips"),
                    Pair("sip_header", "priority=urgent"),
                    Pair("sip_header", "subject=sales%20meeting"),
                    Pair("userinfo", "alice"),
                    Pair("params", ";transport=udp;user=phone;method=REGISTER"),
                    Pair("port", "5061")));
}
