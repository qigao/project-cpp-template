#pragma once

#include <string>

inline std::string getStr(char const* beg, char const* end)
{
    return std::string(beg).substr(0, end - beg);
}
