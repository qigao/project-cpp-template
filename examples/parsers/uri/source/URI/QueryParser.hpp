//
//  QueryParser.hpp
//  File file is part of the "URI" project and released under the MIT License.
//
//  Created by Samuel Williams on 17/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <map>
#include <string>

namespace URI
{
namespace QueryParser
{
using Byte = unsigned char;
using NamedValues = std::multimap<std::string, std::string>;

std::size_t parse(Byte const* begin, Byte const* end,
                  NamedValues& named_values);
} // namespace QueryParser
} // namespace URI
