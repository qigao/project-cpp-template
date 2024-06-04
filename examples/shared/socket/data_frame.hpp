#ifndef __DATA_FRAME_H__
#define __DATA_FRAME_H__

#include "utils/types.h"

#include <memory>

typedef struct DataFrame
{
    std::shared_ptr<u8> data;

    size_t len;

} DataFrame;

#endif // __DATA_FRAME_H__
