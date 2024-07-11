#include "hash_function.hpp"

#include "fs.hpp"

#include <cstring>
#include <openssl/evp.h>
class hash_function::hashImpl
{
public:
    std::string sha_256_hash(char const* input, int len)
    {
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hashLen;
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
        EVP_DigestUpdate(ctx, input, len);
        EVP_DigestFinal_ex(ctx, hash, &hashLen);
        EVP_MD_CTX_free(ctx);
        return to_hex(hash, hashLen);
    }

    std::string md5(char const* input, int len)
    {
        EVP_MD_CTX* context = EVP_MD_CTX_new();
        const EVP_MD* md = EVP_md5();
        unsigned char md_value[EVP_MAX_MD_SIZE];
        unsigned int md_len;

        EVP_DigestInit_ex2(context, md, nullptr);
        EVP_DigestUpdate(context, input, len);
        EVP_DigestFinal_ex(context, md_value, &md_len);
        EVP_MD_CTX_free(context);

        return to_hex(md_value, md_len);
    }
    std::string sha1_hash(char const* input, int len)
    {
        EVP_MD_CTX* context = EVP_MD_CTX_new();
        if (context == nullptr)
        {
            throw std::runtime_error("EVP_MD_CTX_new failed");
        }

        if (EVP_DigestInit_ex(context, EVP_sha1(), nullptr) != 1)
        {
            EVP_MD_CTX_free(context);
            throw std::runtime_error("EVP_DigestInit_ex failed");
        }

        if (EVP_DigestUpdate(context, input, len) != 1)
        {
            EVP_MD_CTX_free(context);
            throw std::runtime_error("EVP_DigestUpdate failed");
        }

        unsigned char digest[EVP_MAX_MD_SIZE];
        unsigned int digest_length;
        if (EVP_DigestFinal_ex(context, digest, &digest_length) != 1)
        {
            EVP_MD_CTX_free(context);
            throw std::runtime_error("EVP_DigestFinal_ex failed");
        }

        EVP_MD_CTX_free(context);

        return to_hex(digest, digest_length);
    }
    hashImpl() {}

private:
};

hash_function::hash_function() : pimpl(std::make_unique<hashImpl>()) {}
hash_function::~hash_function() {}

std::string hash_function::sha_256_hash(std::string const& data)
{
    return pimpl->sha_256_hash(data.c_str(), data.length());
}

std::string hash_function::sha_256_hash(char const* data)
{
    return pimpl->sha_256_hash(data, strlen(data));
}
std::string hash_function::sha1_hash(std::string const& data)
{
    return pimpl->sha1_hash(data.c_str(), data.length());
}

std::string hash_function::sha1_hash(char const* data)
{
    return pimpl->sha1_hash(data, strlen(data));
}

std::string hash_function::md5_hash(char const* data)
{
    return pimpl->md5(data, strlen(data));
}
std::string hash_function::md5_hash(std::string const& data)
{
    return pimpl->md5(data.c_str(), data.length());
}