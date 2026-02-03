#include "Core/Log.h"
#include "pch.h"
#include <chrono>

namespace Engine {

class Timer {

public:
  Timer(const char *name) : mName(name) { reset(); }

  ~Timer() {
    if (!mStoped)
      stop();
  }

  void reset() { mStart = std::chrono::high_resolution_clock::now(); }

  float elapsed() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mStart)
               .count() *
           0.001f * 0.001f * 0.001f;
  }

  float elapsedMillis() { return elapsed() * 1000.0f; }

  void stop() { CORE_TRACE("Duration"); }

private:
  const char *mName;
  std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
  bool mStoped;
};

}  // namespace Engine
