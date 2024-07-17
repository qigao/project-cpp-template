#include "hash_function.hpp"

#include "fs.hpp"
#include "hash_singleton.hpp"

#include <cstring>
#include <memory>

std::string hash_function::sha_256_hash(std::string const& data) const noexcept
{
    HashFunc::Construct();
    return HashFunc::GetInstance()->sha_256_hash(data.c_str(), data.length());
}

std::string hash_function::sha_256_hash(char const* data) const noexcept
{
    HashFunc::Construct();
    return HashFunc::GetInstance()->sha_256_hash(data, strlen(data));
}
std::string hash_function::sha1_hash(std::string const& data) const noexcept
{
    HashFunc::Construct();

    return HashFunc::GetInstance()->sha1_hash(data.c_str(), data.length());
}

std::string hash_function::sha1_hash(char const* data) const noexcept
{
    HashFunc::Construct();

    return HashFunc::GetInstance()->sha1_hash(data, strlen(data));
}

std::string hash_function::md5_hash(char const* data) const noexcept
{
    HashFunc::Construct();

    return HashFunc::GetInstance()->md5(data, strlen(data));
}
std::string hash_function::md5_hash(std::string const& data) const noexcept
{
    HashFunc::Construct();

    return HashFunc::GetInstance()->md5(data.c_str(), data.length());
}
