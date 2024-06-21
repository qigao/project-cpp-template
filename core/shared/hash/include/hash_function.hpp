#ifndef __SODIUM_HASH_H__
#define __SODIUM_HASH_H__

#include <memory>
#include <string>
class hash_function
{
public:
    hash_function();
    ~hash_function();
    std::string sha_256_hash(std::string const& data);
    std::string sha_256_hash(char const* data);

private:
    class hashImpl;
    std::unique_ptr<hashImpl> pimpl;
};
#endif // __SODIUM_HASH_H__
