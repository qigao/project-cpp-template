//
//  Query.cpp
//  File file is part of the "URI" project and released under the MIT License.
//
//  Created by Samuel Williams on 17/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Query.hpp"

#include "QueryParser.hpp"

#include <iostream>

namespace URI
{
Query Query::operator+(Query const& other)
{
    // Ensure neither side is empty:
    if (empty())
        return other;
    if (other.empty())
        return *this;

    // Join them together:
    return value + '&' + other.value;
}

std::multimap<std::string, std::string> Query::to_map() const
{
    using namespace QueryParser;

    NamedValues named_values;

    auto result = parse((Byte const*)value.data(),
                        (Byte const*)value.data() + value.size(), named_values);

    if (result != value.size())
        throw std::invalid_argument("could not parse entire query string");

    return named_values;
}

std::ostream& operator<<(std::ostream& output, Query const& query)
{
    if (query.empty())
        return output;

    return output << '?' << query.value;
}
} // namespace URI
