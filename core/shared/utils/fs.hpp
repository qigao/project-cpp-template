#ifndef CPP_CORE_PROJECT_FS_HPP
#define CPP_CORE_PROJECT_FS_HPP

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

inline int64_t get_file_size(std::string const& filename)
{
    std::ifstream infile(filename.c_str(),
                         std::ios_base::in | std::ios_base::binary);
    if (!infile)
    {
        // throw std::runtime_error("failed to open " + filename + " for
        // read!");
        return 0;
    }
    auto pos0 = infile.tellg();
    infile.seekg(0, std::ios_base::end);
    auto pos = infile.tellg();
    std::streamoff size = pos - pos0;
    infile.close();

    return size;
}

inline void rename(std::string const& oname, std::string const& nname)
{
    if (std::rename(oname.c_str(), nname.c_str()) != 0)
    {
        throw std::runtime_error("failed to rename " + oname + " => " + nname);
    }
}

inline void append(std::string const& filename, char const* data, size_t size)
{
    std::ofstream outfile(filename.c_str(),
                          std::ios_base::out | std::ios_base::app);
    if (!outfile)
    {

        throw std::runtime_error("failed to open " + filename + " for write");
    }
    std::cout << "fs::append(" << filename << ", "
              << static_cast<void const*>(data) << ", " << size << ")"
              << std::endl;
    outfile.write(data, static_cast<long>(size));
}

inline void write(std::string const& filename, char const* data, size_t size)
{
    std::ofstream outfile(filename.c_str(),
                          std::ios_base::out | std::ios_base::trunc);
    if (!outfile)
    {
        throw std::runtime_error("failed to open " + filename + " for write");
    }
    outfile.write(data, static_cast<long>(size));
}

inline bool breakup_url(std::string const& url, std::string& scheme_and_host,
                        std::string& path)
{
    std::string::size_type off = 0;
    std::string scheme_notation = "://";
    std::string::size_type pos = url.find(scheme_notation, off);
    if (pos == std::string::npos)
    {
        return false;
    }
    off = pos + scheme_notation.length();
    pos = url.find('/', off);
    if (pos == std::string::npos)
    {
        pos = url.length();
    }
    scheme_and_host = url.substr(0, pos);
    path = url.substr(pos);
    if (path.empty())
    {
        path = "/";
    }
    return true;
}

#endif // CPP_CORE_PROJECT_FS_HPP
