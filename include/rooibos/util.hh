#ifndef ROOIBOS_UTIL_HH
#define ROOIBOS_UTIL_HH

#include <memory>
#include <utility>

namespace rooibos
{
  template<class T, class... Args>
  std::unique_ptr<T> make_unique(Args&&... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

  void panic(const std::string & msg) __attribute__((noreturn));
}

#endif
