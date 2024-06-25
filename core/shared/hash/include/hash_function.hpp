#ifndef __HASH_FUNCTION_H__
#define __HASH_FUNCTION_H__

#include <memory>
#include <string>

class hash_function
{
public:
    hash_function();
    ~hash_function();
    std::string sha_256_hash(std::string const& data);
    std::string sha_256_hash(char const* data);
    std::string sha1_hash(std::string const& data);
    std::string sha1_hash(char const* data);
    std::string md5_hash(char const* data);
    std::string md5_hash(std::string const& data);

private:
    class hashImpl;
    std::unique_ptr<hashImpl> pimpl;
};

#endif // __HASH_FUNCTION_H__