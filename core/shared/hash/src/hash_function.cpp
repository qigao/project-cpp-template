#include "hash_function.hpp"
#include <fmt/core.h>
#include <iterator>
#include <range/v3/all.hpp>
#include <sodium.h>
#include <sodium/crypto_hash_sha256.h>

class hash_function::hashImpl
{
public:
    bool sha_256_hash_sodium(char const* data, int len, unsigned char* out)
    {
        int result = crypto_hash_sha256(
            out, reinterpret_cast<unsigned char const*>(data), len);
        return result == 0;
    }
    std::string bin2hex_sodium(unsigned char* hash, int len_hash)
    {
        unsigned int const max_hash_len = crypto_hash_sha256_BYTES * 2 + 1;
        char hashHex[max_hash_len];
        sodium_bin2hex(hashHex, max_hash_len, hash, len_hash);
        std::string result;
        ranges::copy(hashHex, hashHex + max_hash_len - 1,
                     std::back_inserter(result));
        return result;
    }
    hashImpl()
    {
        if (sodium_init() < 0)
        {
            throw std::runtime_error("Failed to initialize hash ");
        }
    }
};

hash_function::hash_function() : pimpl(new hashImpl) {}
hash_function::~hash_function() {}

std::string hash_function::sha_256_hash(std::string const& data)
{
    unsigned char hash[crypto_hash_sha256_BYTES];
    auto result = pimpl->sha_256_hash_sodium(data.c_str(), data.length(), hash);
    if (result)
    {
        return pimpl->bin2hex_sodium(hash, crypto_hash_sha256_BYTES);
    }
    else
    {
        return "";
    }
}

std::string hash_function::sha_256_hash(char const* data)
{
    unsigned char hash[crypto_hash_sha256_BYTES];
    auto result = pimpl->sha_256_hash_sodium(data, strlen(data), hash);
    if (result)
    {
        return pimpl->bin2hex_sodium(hash, crypto_hash_sha256_BYTES);
    }
    else
    {
        return "";
    }
}