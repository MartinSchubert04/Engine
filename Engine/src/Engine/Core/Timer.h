#include "Core/Log.h"
#include "pch.h"
#include <chrono>

namespace Engine {

template <typename Fn>
class Timer {

public:
  Timer(const char *name, Fn &&func) : mName(name), mFunc(func), mStoped(false) { reset(); }

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

  void stop() {
    auto duration = elapsedMillis();
    mFunc({mName, duration});
  }

private:
  const char *mName;
  std::chrono::time_point<std::chrono::high_resolution_clock> mStart;
  bool mStoped;
  Fn mFunc;
};

}  // namespace Engine
