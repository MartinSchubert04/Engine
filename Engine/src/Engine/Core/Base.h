#pragma once

#include "Core/PlataformDetection.h"

#include <memory>
#include <utility>

// displace 1 by x positions, always (e.j, x=1  BIT output 2, x=2 BIT output 4 )

#define DEBUG

#ifdef DEBUG
  #if defined(PLATFORM_WINDOWS)
    #define DEBUG_BREAK() __builtin_trap()
  #elif defined(PLATFORM_LINUX)
    #include <signal.h>
    #define DEBUGBREAK() raise(SIGTRAP)
  #else
    #error "Platform doesn't support debugbreak yet!"
  #endif
  #define ENABLE_ASSERTS
#else
  #define DEBUG_BREAK()
#endif

#define BIT(x) (1 << x)

#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x

/*
#define BIND_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
*/

#define BIND_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Engine {

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> createScope(Args &&...args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> createRef(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

}  // namespace Engine

#include "Core/Log.h"
#include "Core/Assert.h"
