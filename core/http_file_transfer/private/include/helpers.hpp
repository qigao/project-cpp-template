
#ifndef CPP_CORE_PROJECT_HTTP_HELPERS_HPP
#define CPP_CORE_PROJECT_HTTP_HELPERS_HPP
#include <fmt/core.h>
#include <sstream>
#include <string>
inline int some_fun()
{
    fmt::print("Hello {} !", "helpers");
    return 0;
}

template <typename U, typename V>
struct sstream_cast_impl
{
    static V do_cast(U const& u)
    {
        V v;
        std::stringstream ss;
        ss << u;
        ss >> v;
        return v;
    }
};

template <typename U>
struct sstream_cast_impl<U, std::string>
{
    static std::string do_cast(U const& u)
    {
        std::ostringstream oss;
        oss << u;
        return oss.str();
    }
};

template <typename V, typename U>
V sstream_cast(U const& u)
{
    return sstream_cast_impl<U, V>::do_cast(u);
}

template <typename V>
V sstream_cast(char const* s)
{
    return sstream_cast_impl<char const*, V>::do_cast(s);
}
#endif // CPP_CORE_PROJECT_HTTP_HELPERS_HPP
