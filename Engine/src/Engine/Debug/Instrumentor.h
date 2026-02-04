#pragma once

#include "Core/Log.h"
#include "pch.h"

namespace Engine {

struct ProfileResult {
  const char *name;
  float time;
};

class Instrumentor {
  // TODO imgui instrumentor
public:
  static void addResult(ProfileResult profile) { mProfileResults.push_back(profile); }
  static void clearResults() { mProfileResults.clear(); }
  static std::vector<ProfileResult> &getProfileResults() { return mProfileResults; }

private:
  static std::vector<ProfileResult> mProfileResults;
};

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

#if PROFILING

  #define PROFILE_SCOPE(name) \
    Timer timer##__LINE__(name, [&](ProfileResult profileResult) { Instrumentor::addResult(profileResult); })

#else
  #define PROFILE_SCOPE(name)
#endif
