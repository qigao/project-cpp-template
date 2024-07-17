#ifndef __DUMMY_FUNCTION_H__
#define __DUMMY_FUNCTION_H__

#include "hash_base.hpp"
#include "random.hpp"

class DummyImpl : public HashImpl
{
public:
    std::string sha_256_hash(char const* /* input */, int /* len */) override
    {
        return random_string(64);
    }

    std::string md5(char const* input, int len) override
    {
        return random_string(32);
    }
    std::string sha1_hash(char const* input, int len) override
    {
        return random_string(40);
    }
};

#endif // __DUMMY_FUNCTION_H__
