#include <chrono>

struct ScopedTimer {
  const char *name;
  std::chrono::high_resolution_clock::time_point start;

  ScopedTimer(const char *name)
      : name(name), start(std::chrono::high_resolution_clock::now()) {}

  ~ScopedTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    float ms = std::chrono::duration<float, std::milli>(end - start).count();
    printf("[TIMER] %s: %.3f ms\n", name, ms);
  }
};
