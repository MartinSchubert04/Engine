#include "common.h"

namespace Engine {

enum class EventType {
  NONE = 0,
  window_closed,
  window_focus,
  window_lost_focus,
  window_moved,
  app_tick,
  app_update,
  KeyPressed,
  KeyReleased,
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

#define EVENT_CLASS_TYPE(type) \
  static EventType GetStaticType() { \
    return EventType::type; \
  } \
  virtual EventType GetEventType() const override { \
    return GetStaticType(); \
  } \
  virtual const char *GetName() const override { \
    return #type; \
  }

#define EVENT_CLASS_CATEGORY(category) \
  virtual int GetCategoryFlags() const override { \
    return category; \
  }

class Event {

public:
  friend class EventDispatcher;

protected:
  bool mHandled = false;

public:
  virtual EventType getEventType() const = 0;
  virtual const char *getName() const = 0;
  virtual int getCategoryFlags() const = 0;
  virtual std::string toString() const { return getName(); }

  bool isInCategory(EventCategory category) { return category & getCategoryFlags(); }
};

class EventDispatcher {
private:
  Event &mEvent;

public:
  EventDispatcher(Event &event) : mEvent(event) {}

  template <typename T, typename F>
  void dispatch(const F &func) {}
};

}  // namespace Engine
