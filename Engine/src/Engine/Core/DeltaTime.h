#pragma once

namespace Engine {
class DeltaTime {
public:
  DeltaTime(float time = 0.0f) : mTime(time) {};

  float getSeconds() { return mTime; }
  float getMilliseconds() { return mTime * 1000; }

private:
  float mTime;
};

}  // namespace Engine
