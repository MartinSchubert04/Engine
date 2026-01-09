#include "Event.h"

namespace Engine {

class WindowResizeEvent : public Event {

public:
  WindowResizeEvent(unsigned int width, unsigned int height) : mWidth(width), mHeight(height) {}

  unsigned int GetWidth() const { return mWidth; }
  unsigned int GetHeight() const { return mHeight; }

  std::string toString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
  unsigned int mWidth, mHeight;
};

class WindowCloseEvent : public Event {

public:
  WindowCloseEvent() = default;

  EVENT_CLASS_TYPE(WindowClosed)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

// App Events might not be implemented, not truly necesary
class AppTickEvent : public Event {
public:
  AppTickEvent() = default;

  EVENT_CLASS_TYPE(AppTick)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event {
public:
  AppUpdateEvent() = default;

  EVENT_CLASS_TYPE(AppUpdate)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event {
public:
  AppRenderEvent() = default;

  EVENT_CLASS_TYPE(AppRender)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

}  // namespace Engine
