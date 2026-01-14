#pragma once
#include "pch.h"
#include "Core/Base.h"

namespace Engine {

enum class EventType {
  NONE = 0,
  WindowClosed,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  AppTick,
  AppRender,
  AppUpdate,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

enum EventCategory {
  NONE = 0,
  EventCategoryApplication = BIT(0),
  EventCategoryInput = BIT(1),
  EventCategoryKeyboard = BIT(2),
  EventCategoryMouse = BIT(3),
  EventCategoryMouseButton = BIT(4),
};

// defs allow to not re type every func inside the child classes
#define EVENT_CLASS_TYPE(type) \
  static EventType getStaticType() { \
    return EventType::type; \
  } \
  virtual EventType getEventType() const override { \
    return getStaticType(); \
  } \
  virtual const char *getName() const override { \
    return #type; \
  }

#define EVENT_CLASS_CATEGORY(category) \
  virtual int getCategoryFlags() const override { \
    return category; \
  }

class Event {

  friend class EventDispatcher;

public:
  bool handled = false;

  virtual EventType getEventType() const = 0;
  virtual const char *getName() const = 0;
  virtual int getCategoryFlags() const = 0;
  virtual std::string toString() const { return getName(); }

  bool isInCategory(EventCategory category) { return category & getCategoryFlags(); }
};

class EventDispatcher {

public:
  EventDispatcher(Event &event) : mEvent(event) {}

  template <typename T, typename F>
  bool dispatch(const F &func) {

    if (mEvent.getEventType() == T::getStaticType()) {
      mEvent.handled |= func(static_cast<T &>(mEvent));

      return true;
    }
    return false;
  }

private:
  Event &mEvent;
};

inline std::ostream &operator<<(std::ostream &os, const Event &e) {
  return os << e.toString();
}

}  // namespace Engine
