#ifndef __HASH_BASE_H__
#define __HASH_BASE_H__

#include <string>
class HashImpl
{
public:
    virtual std::string sha_256_hash(char const* input, int len) = 0;
    virtual std::string md5(char const* input, int len) = 0;
    virtual std::string sha1_hash(char const* input, int len) = 0;
};

#endif // __HASH_BASE_H__
