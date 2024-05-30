#ifndef __INPUT_READER_H__
#define __INPUT_READER_H__

#include <iostream>
#include <sstream>
namespace Parser {
  class Reader
  {
  public:
    Reader() {}
    virtual ~Reader() {}

    virtual size_t            read(char *buffer, size_t size) = 0;
    virtual const std::string name() const                    = 0;
    virtual bool              valid() const                   = 0;
    virtual bool              end() const                     = 0;
  };

  class StreamReader : public Reader
  {
  public:
    StreamReader(std::istream &stream) : stream(stream) {}
    virtual ~StreamReader() {}

    virtual size_t read(char *buffer, size_t size)
    {
      this->stream.read(buffer, size);
      return this->stream.gcount();
    }

    virtual bool valid() const { return this->stream.good(); }

    virtual bool end() const { return this->stream.eof(); }

    virtual const std::string name() const { return "stream"; }

  private:
    std::istream &stream;
  };
}// namespace Parser

#endif// __INPUT_READER_H__
