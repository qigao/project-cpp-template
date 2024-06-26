//
//  Generic.cpp
//  File file is part of the "Uri" project and released under the MIT License.
//
//  Created by Samuel Williams on 13/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Generic.hpp"

#include "GenericParser.hpp"

#include <stdexcept>

namespace URI
{
Generic::Generic(char const* begin, char const* end)
{
    using namespace GenericParser;

    auto result =
        GenericParser::parse((unsigned char*)begin, (unsigned char*)end, *this);

    if (result != std::size_t(end - begin))
        throw std::invalid_argument("could not parse entire uri string");
}

std::ostream& operator<<(std::ostream& output, Generic const& generic)
{
    if (!generic.scheme.empty())
    {
        output << generic.scheme << ":";
    }

    if (!generic.host.empty())
    {
        output << "//";

        if (!generic.userinfo.empty())
        {
            output << generic.userinfo << '@';
        }

        output << generic.host;

        if (!generic.port.empty())
        {
            output << ':' << generic.port;
        }
    }

    output << generic.path;
    output << generic.query;

    if (!generic.fragment.empty())
    {
        output << '#' << generic.fragment;
    }

    return output;
}

bool Generic::operator==(Generic const& other) const noexcept
{
    return scheme == other.scheme && userinfo == other.userinfo &&
           host == other.host && port == other.port && path == other.path &&
           query == other.query && fragment == other.fragment;
}

bool Generic::operator!=(Generic const& other) const noexcept
{
    return !(*this == other);
}

bool Generic::operator<(Generic const& other) const noexcept
{
    if (scheme < other.scheme)
        return true;

    if (userinfo < other.userinfo)
        return true;

    if (host < other.host)
        return true;

    if (port < other.port)
        return true;

    if (path < other.path)
        return true;

    if (query < other.query)
        return true;

    if (fragment < other.fragment)
        return true;

    return false;
}

Generic Generic::operator+(Generic const& other) const
{
    if (other.is_absolute())
    {
        return other;
    }

    Generic result = *this;

    result.query = other.query;
    result.fragment = other.fragment;

    if (other.path.is_absolute())
    {
        result.path = other.path;
    }
    else
    {
        result.path = path + other.path;
    }

    return result;
}

std::string Generic::hostname() const
{
    // Must have at least 2 characters to unwrap:
    if (host.size() < 2)
        return host;

    if (host.front() == '[' && host.back() == ']')
        return host.substr(1, host.size() - 2);
    else
        return host;
}
} // namespace URI
