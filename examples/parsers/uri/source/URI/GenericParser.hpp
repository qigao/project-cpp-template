//
//  GenericParser.hpp
//  File file is part of the "URI" project and released under the MIT License.
//
//  Created by Samuel Williams on 15/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Generic.hpp"

namespace URI
{
namespace GenericParser
{
using Byte = unsigned char;

std::size_t parse(Byte const* begin, Byte const* end, Generic& generic);
} // namespace GenericParser
} // namespace URI
