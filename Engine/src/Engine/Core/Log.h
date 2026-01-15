#pragma once

#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"
#include "Core/Base.h"
#include "pch.h"

namespace Engine {

class Log {
public:
  static void init();

  inline static std::shared_ptr<spdlog::logger> &getCoreLogger() { return mCoreLogger; }
  inline static std::shared_ptr<spdlog::logger> &getClientLogger() { return mClientLogger; }

private:
  static std::shared_ptr<spdlog::logger> mCoreLogger;
  static std::shared_ptr<spdlog::logger> mClientLogger;
};

}  // namespace Engine

// Core logger macros
#define CORE_TRACE(...) ::Engine::Log::getCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...) ::Engine::Log::getCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...) ::Engine::Log::getCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::Engine::Log::getCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...) ::Engine::Log::getCoreLogger()->fatal(__VA_ARGS__)

// Client logger macros
#define E_TRACE(...) ::Engine::Log::getClientLogger()->trace(__VA_ARGS__)
#define E_INFO(...) ::Engine::Log::getClientLogger()->info(__VA_ARGS__)
#define E_WARN(...) ::Engine::Log::getClientLogger()->warn(__VA_ARGS__)
#define E_ERROR(...) ::Engine::Log::getClientLogger()->error(__VA_ARGS__)
#define E_FATAL(...) ::Engine::Log::getClientLogger()->fatal(__VA_ARGS__)
