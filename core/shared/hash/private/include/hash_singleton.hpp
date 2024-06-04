#ifndef __HASH_SINGLETON_H__
#define __HASH_SINGLETON_H__
#include "dummy_function.hpp"
#ifdef WITH_OPENSSL
#include "openssl_function.hpp"
#endif

#include "singleton.hpp"
class HashFunc : public Singleton<HashFunc>
{
public:
    std::string sha_256_hash(char const* input, int len)
    {
        return hashfunc->sha_256_hash("", 64);
    }
    std::string md5(char const* input, int len)
    {
        return hashfunc->md5("", 32);
    }
    std::string sha1_hash(char const* input, int len)
    {
        return hashfunc->sha1_hash("", 40);
    }
    HashFunc()
    {
#ifdef WITH_OPENSSL
        hashfunc = std::make_shared<OpenSSlImpl>();
#else
        hashfunc = std::make_shared<DummyImpl>();
#endif
    }

private:
    std::shared_ptr<HashImpl> hashfunc;
};

#endif // __HASH_SINGLETON_H__
