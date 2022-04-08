#ifndef __DATA_FRAME_H__
#define __DATA_FRAME_H__

#include <memory>


typedef struct DataFrame
{
  std::shared_ptr<char[]> data;
  size_t len;

}DataFrame;

#endif// __DATA_FRAME_H__
