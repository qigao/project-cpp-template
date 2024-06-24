//
//  Query.hpp
//  File file is part of the "URI" project and released under the MIT License.
//
//  Created by Samuel Williams on 17/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Encoding.hpp"

#include <map>
#include <string>

namespace URI
{
/// Assumes application/x-www-form-urlencoded.
struct Query
{
    std::string value;

    Query() {}

    template <typename ValueT>
    Query(ValueT const& value_) : value(value_)
    {
    }

    template <typename IteratorT>
    Query(IteratorT begin, IteratorT end)
        : Query(Encoding::encode_query(begin, end))
    {
    }

    bool empty() const noexcept { return value.empty(); }
    explicit operator bool() const noexcept { return !empty(); }

    std::multimap<std::string, std::string> to_map() const;

    Query operator+(Query const& other);

    bool operator==(Query const& other) const { return value == other.value; }
    bool operator!=(Query const& other) const { return value != other.value; }
    bool operator<(Query const& other) const { return value < other.value; }
    bool operator<=(Query const& other) const { return value <= other.value; }
    bool operator>(Query const& other) const { return value > other.value; }
    bool operator>=(Query const& other) const { return value >= other.value; }
};

std::ostream& operator<<(std::ostream& output, Query const& query);
} // namespace URI
