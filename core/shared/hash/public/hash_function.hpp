
#include "hashfunc_export.h"

#include <memory>
#include <string>

class HASHFUNC_EXPORT hash_function
{
public:
    std::string sha_256_hash(std::string const& data) const noexcept;
    std::string sha_256_hash(char const* data) const noexcept;
    std::string sha1_hash(std::string const& data) const noexcept;
    std::string sha1_hash(char const* data) const noexcept;
    std::string md5_hash(char const* data) const noexcept;
    std::string md5_hash(std::string const& data) const noexcept;
};
