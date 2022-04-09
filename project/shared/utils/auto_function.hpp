#ifndef __AUTO_FUNCTION_H__
#define __AUTO_FUNCTION_H__

#include <functional>

template<typename Fn, typename... Args>
decltype(auto) Call(Fn &&fun, Args &&...args)
{
  return std::invoke(std::forward<Fn>(fun), std::forward<Args>(args)...);
}

#endif// __AUTO_FUNCTION_H__
